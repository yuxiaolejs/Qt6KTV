#!/usr/bin/env node
const fs = require("fs");
const mysql = require("mysql2/promise");
const pool = mysql.createPool(require("./credentials.json").sql);
const crypto = require("crypto");
const pLimit = require("p-limit").default;

if (process.argv.length < 3) {
  console.error("Usage: node import.js <path_to_media_directory>");
  process.exit(1);
}

const mediaPath = process.argv[2];

// limit how many files are hashed in parallel
const limit = pLimit(4); // tune: 4–8 usually safe

function getFileMd5(filePath) {
  return new Promise((resolve, reject) => {
    const hash = crypto.createHash("md5");
    const stream = fs.createReadStream(filePath);
    stream.on("data", (data) => hash.update(data));
    stream.on("end", () => resolve(hash.digest("hex")));
    stream.on("error", (err) => reject(err));
  });
}

async function readDirRecursively(dir) {
  let files = await fs.promises.readdir(dir, { withFileTypes: true });
  let mediaFiles = [];

  // process all entries in this directory in parallel
  await Promise.all(
    files.map(async (file) => {
      let fullPath = `${dir}/${file.name}`;
      if (file.isDirectory()) {
        let subFiles = await readDirRecursively(fullPath);
        mediaFiles.push(...subFiles);
      } else if (file.isFile()) {
        // run the whole task under concurrency limit
        await limit(async () => {
          // Check if file already exists in DB
          let [existing] = await pool.query(
            "SELECT * FROM media WHERE name = ?",
            [file.name]
          );
          if (existing.length > 0) {
            console.log(`File already exists in database: ${file.name}`);
            return;
          }

          // Gather metadata
          const stats = await fs.promises.stat(fullPath);
          const md5 = await getFileMd5(fullPath);

          const media = {
            name: file.name,
            fullPath: fullPath,
            size: stats.size,
            md5: md5,
          };
          mediaFiles.push(media);

          // Insert into DB
          try {
            await pool.query(
              "INSERT INTO media (name, path, size, count, md5) VALUES (?, ?, ?, 0, ?)",
              [media.name, media.fullPath, media.size, media.md5]
            );
            console.log(`Inserted: ${media.name}`);
          } catch (error) {
            console.error(`Error inserting ${media.name}:`, error.message);
          }
        });
      }
    })
  );

  return mediaFiles;
}

function main() {
  console.log(`Importing media files from ${mediaPath}...`);
  readDirRecursively(mediaPath)
    .then(async (mediaFiles) => {
      console.log(`\nImported ${mediaFiles.length} new files.`);
      console.log("Import completed.");
      process.exit(0);
    })
    .catch((error) => {
      console.error("Error during import:", error.message);
      process.exit(1);
    });
}

main();
