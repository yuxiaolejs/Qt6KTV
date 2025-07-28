const express = require("express");
const fs = require("fs");
const app = express();
const mysql = require("mysql2/promise");
const { Throttle } = require('stream-throttle');

const pool = mysql.createPool(require("./credentials.json").sql);

app.all(/\/api\/v1(.*)/, async (req, res, next) => {
  if (!req.headers.authorization || req.headers.authorization == "")
    return res.status(401).send("Unauthorized");
  let auth = req.headers.authorization.split(":");
  if (auth.length != 2) {
    return res.status(401).send("Unauthorized");
  }
  const [user] = await pool.query("SELECT * FROM users WHERE username = ? AND token = ?", [
    auth[0],
    auth[1],
  ])
  if (user.length === 0) {
    return res.status(401).send("Unauthorized");
  }
  req.user = user[0];
  console.log("User authenticated:", req.user.username);
  next();
});

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
    [list[0].id, req.user.id, list[0].size, new Date()]
  );
  // Add size in header
  res.setHeader("Content-Length", list[0].size);
  res.setHeader("Content-Type", "application/octet-stream");
  // Add md5 of the file in header
  res.setHeader("Content-MD5", list[0].md5);
  res.writeHead(200)
  let readPipe = fs.createReadStream(list[0].path);
  const throttle = new Throttle({ rate: 50 * 1024 * 1024 }); // 50 MB/s
  readPipe.pipe(throttle).pipe(res);
});

app.listen(18010);
