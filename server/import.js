const fs = require("fs");
const mysql = require("mysql2/promise");
const pool = mysql.createPool(require("./credentials.json").sql);

if (process.argv.length < 3) {
  console.error("Usage: node import.js <path_to_media_directory>");
  process.exit(1);
}

const mediaPath = process.argv[2];

async function readDirRecursively(dir) {
  let files = await fs.promises.readdir(dir, { withFileTypes: true });
  let mediaFiles = [];

  for (let file of files) {
    let fullPath = `${dir}/${file.name}`;
    if (file.isDirectory()) {
      mediaFiles.push(...(await readDirRecursively(fullPath)));
    } else if (file.isFile()) {
      mediaFiles.push({
        name: file.name,
        fullPath: fullPath,
        size: (await fs.promises.stat(fullPath)).size,
      });
    }
  }

  return mediaFiles;
}

async function insertAll(mediaFiles) {
  for (let media of mediaFiles) {
    try {
      await pool.query(
        "INSERT INTO media (name, path, size, count) VALUES (?, ?, ?, 0)",
        [media.name, media.fullPath, media.size]
      );
      console.log(`Inserted: ${media.name}`);
    } catch (error) {
      console.error(`Error inserting ${media.name}:`, error.message);
    }
  }
}

function main() {
  console.log(`Importing media files from ${mediaPath}...`);
  readDirRecursively(mediaPath).then(async (mediaFiles) => {
    console.log(mediaFiles);
    return insertAll(mediaFiles);
  }).then(() => {
    console.log("Import completed.");
    process.exit(0);
  }).catch((error) => {
    console.error("Error during import:", error.message);
    process.exit(1);
  });
}

main();
