const electron = require("electron");
const child_process = require("child_process");
const prompt = require("electron-prompt");

const { app, BrowserWindow, ipcMain, ipcRenderer } = electron;

let mainWindow;

app.on("ready", function() {
  mainWindow = new BrowserWindow({});
  mainWindow.loadFile("mainWindow.html");
  prompt({
    title: "Server IP address",
    label: "IP:",
    value: "127.0.0.1",
    inputAttrs: {
      type: "url"
    }
  })
    .then(r => {
      if (r === null) {
        console.log("user cancelled");
      } else {
        console.log("result", r);
        spawnCoreCpp(r);
      }
    })
    .catch(console.error);
  console.log(__dirname.replace(" ", "\\ "));
});

function spawnCoreCpp(ipaddr) {
  const subprocess = child_process.spawn(process.env.SHELL, [
    "-c",
    "cd " + __dirname.replace(" ", "\\ ") + " && ./a.out " + ipaddr
  ]);

  subprocess.stdout.on("data", data => {
    console.log(`stdout: ${data.toString().split("\n")}`);
    let inArr = data.toString().split("\n");
    inArr.forEach(element => {
      let sigleMessage = element.split(":::");
      if (sigleMessage.length === 3) {
        mainWindow.webContents.send("update", sigleMessage);
      } else {
      }
    });
  });

  subprocess.on("close", () => {
    console.log("closed");
  });
  ipcMain.on("input", (e, item) => {
    console.log("input:", item);
    subprocess.stdin.write(item + "\n");
  });
  app.on("quit", (e, code) => {
    subprocess.stdin.write("~!" + "\n");
  });
}
