#ifndef HOMEPAGE_H
#define HOMEPAGE_H

const char homePagePart1[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Sensor Dashboard</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f4f4f4;
      color: #333;
      display: flex;
      flex-direction: column;
      align-items: center;
      margin: 0;
      padding: 0;
    }
    h1 {
      text-align: center;
      margin-top: 20px;
    }
    .container {
      display: flex;
      flex-wrap: wrap;
      justify-content: center;
      margin: 20px;
    }
    .card {
      background-color: #fff;
      border-radius: 10px;
      box-shadow: 0 4px 6px rgba(0,0,0,0.1);
      margin: 10px;
      padding: 20px;
      min-width: 120px;
      flex: 1 1 150px;
      text-align: center;
    }
    .value {
      font-size: 1.5em;
      font-weight: bold;
      margin-top: 5px;
    }
  </style>
</head>
<body>
  <h1>ESP32 Sensor Dashboard</h1>
  <div class="container" id="sensorContainer">
)rawliteral";


