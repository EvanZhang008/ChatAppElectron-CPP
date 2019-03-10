const electron = require("electron");
const url = require("url");
const assert = require("assert");
const fs = require("fs");
const child_process = require("child_process");

const subprocess = child_process.spawn("../core/a.out", ["client"]);
subprocess.stdout.on("data", data => {
  //   console.log(`stdout: ${data.toString().split("\n")}`);
  let inArr = data.toString().split("\n");
  inArr.forEach(element => {
    let sigleMessage = element.split(":::");
    if (sigleMessage.length === 3) {
      mainWindow.webContents.send("update", sigleMessage);
    } else {
    }
  });
});

// setTimeout(() => {
//   subprocess.stdin.write("1\n");
// }, 1000);

subprocess.on("close", () => {
  console.log("closed");
});
const { app, BrowserWindow, ipcMain, ipcRenderer } = electron;

let mainWindow;

app.on("ready", function() {
  mainWindow = new BrowserWindow({});
  mainWindow.loadFile("mainWindow.html");
});
ipcMain.on("input", (e, item) => {
  console.log("input:", item);
  subprocess.stdin.write(item + "\n");
});

app.on("quit", (e, code) => {
  subprocess.stdin.write("~!" + "\n");
});
