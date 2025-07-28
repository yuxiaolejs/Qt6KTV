const express = require("express");
const fs = require("fs");
const app = express();
const mysql = require("mysql2/promise");

const pool = mysql.createPool(require("./credentials.json").sql);

app.get("/api/v1/media/list", async (req, res) => {
  res.json([]);
});
app.get("/api/v1/media/search", async (req, res) => {
  let [list] = await pool.query(
    "SELECT name FROM media WHERE name LIKE ? ORDER BY count DESC LIMIT 100",
    [`%${req.query.query}%`]
  );
  res.json(list.map((item) => item.name));
});
app.get("/api/v1/media/:id", async (req, res) => {
  let [list] = await pool.query("SELECT * FROM media WHERE name = ? LIMIT 1", [
    req.params.id,
  ]);
  // res.status(404).send("Media not found");
  // return;
  if (list.length === 0) {
    res.status(404).send("Media not found");
    return;
  }
  // make sure the file exists
  try {
    await fs.promises.stat(list[0].path);
  } catch (e) {
    res.status(404).send("File not found");
    return;
  }
  await pool.query(
    "INSERT INTO downloads (mediaid, userid, traffic, time) VALUES (?,?,?,?)",
    [list[0].id, 0, list[0].size, new Date()]
  );
  let readPipe = fs.createReadStream(list[0].path);
  readPipe.pipe(res);
});

app.listen(18010);
