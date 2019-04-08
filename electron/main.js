/*---------------------------------------------------------------------------------------
--	SOURCE FILE:    main.js - The starting point for the electron GUI
--
--	PROGRAM:        Linux Chat
--
--	FUNCTIONS:      function spawnCoreCpp(ipaddr)
--
--	DATE:			March 8, 2019
--
--	REVISIONS:	    (Date and Description)
--
--	DESIGNER:		Ziqian Zhang, Viktor Alvar
--
--	PROGRAMMER:		Ziqian Zhang, Viktor Alvar
--
--	NOTES:
--  The starting point for the electron application. Connects the executables to the
--  GUI. First the Linux Chat C++ source files must be compiled and execuded before
--  running the GUI. Once you create the executable, move it to the same directory as
--  this file. Finally, to run the GUI ensure that npm is installed on your machine
--  by typing "dnf install npm". Afterwards install the dependencies by typing
--  "npm install" and to compile the GUI type "npm start". Type these commands in the
--  same directory as this file.
---------------------------------------------------------------------------------------*/

const electron = require("electron");
const child_process = require("child_process");
const prompt = require("electron-prompt");

const { app, BrowserWindow, ipcMain, ipcRenderer } = electron;
let mainWindow;

// Ready event for the electron app
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

/*---------------------------------------------------------------------------------------
--	FUNCTION:		spawnCoreCpp
--
--	DATE:			March 8, 2019
--
--	REVISIONS:	    (Date and Description)
--
--	DESIGNER:		Ziqian Zhang, Viktor Alvar
--
--	PROGRAMMER:		Ziqian Zhang
--
--	INTERFACE:      function spawnCoreCpp(ipaddr)
--                      ipaddr: Server IP address
--
--	NOTES:
--  Spawns the core c++ executable and connects it to the electron GUI. Runs the
--  executable and add the ip address of the server as the command line argument.
--  Handles the stdin and stdout in the executable and routes them to the GUI.
--  This function is called when user inputs the ip address in the prompt in the GUI.
---------------------------------------------------------------------------------------*/
function spawnCoreCpp(ipaddr) {
  const subprocess = child_process.spawn(process.env.SHELL, [
    "-c",
    "cd " + __dirname.replace(" ", "\\ ") + " && ./linuxchat " + ipaddr
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

  // Close event
  subprocess.on("close", () => {
    console.log("closed");
  });

  // Input event
  // Connects the executable stdin to the GUI
  ipcMain.on("input", (e, item) => {
    console.log("input:", item);
    subprocess.stdin.write(item + "\n");
  });

  // Quit event
  app.on("quit", (e, code) => {
    subprocess.stdin.write("~!" + "\n");
  });
}
