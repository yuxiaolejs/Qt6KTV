const fs = require("fs");
const mysql = require("mysql2/promise");
const pool = mysql.createPool(require("./credentials.json").sql);
const crypto = require("crypto");

if (process.argv.length < 3) {
  console.error("Usage: node import.js <path_to_media_directory>");
  process.exit(1);
}

const mediaPath = process.argv[2];

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

  for (let file of files) {
    let fullPath = `${dir}/${file.name}`;
    if (file.isDirectory()) {
      mediaFiles.push(...(await readDirRecursively(fullPath)));
    } else if (file.isFile()) {
      // First issue a query to check if the file already exists
      let [existing] = await pool.query("SELECT * FROM media WHERE name = ?", [file.name]);
      if (existing.length > 0) {
        console.log(`File already exists in database: ${file.name}`);
        continue; // Skip this file if it already exists
      }
      // If it doesn't exist, proceed to insert it
      const media = {
        name: file.name,
        fullPath: fullPath,
        size: (await fs.promises.stat(fullPath)).size,
        md5: await getFileMd5(fullPath),
      };
      mediaFiles.push(media);
      try {
        await pool.query(
          "INSERT INTO media (name, path, size, count, md5) VALUES (?, ?, ?, 0, ?)",
          [media.name, media.fullPath, media.size, media.md5]
        );
        console.log(`Inserted: ${media.name}`);
      } catch (error) {
        console.error(`Error inserting ${media.name}:`, error.message);
      }
    }
  }

  return mediaFiles;
}


function main() {
  console.log(`Importing media files from ${mediaPath}...`);
  readDirRecursively(mediaPath)
    .then(async (mediaFiles) => {
      console.log(mediaFiles);
      console.log("Import completed.");
      process.exit(0);
    })
    .catch((error) => {
      console.error("Error during import:", error.message);
      process.exit(1);
    });
}

main();
