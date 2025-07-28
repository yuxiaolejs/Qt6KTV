const express = require("express");
const fs = require("fs");
const app = express();

app.get("/api/v1/media/list", (req, res) => {
  console.log("list media request received");
  res.json(["123", "456", "789"]);
});
app.get("/api/v1/media/search", (req, res) => {
  console.log("search media request received");
  res.json(["1123.mkv", req.query.query]);
});
app.get("/api/v1/media/:id", (req, res) => {
  console.log("get media request received for id:", req.params.id);
  let readPipe = fs.createReadStream("/home/unics/Desktop/KTV/04月/RE-D 是二哈ya masta - 肯定[1080P]-国语-流行.mkv");
  readPipe.pipe(res);
});

app.listen(18010);
