#ifndef HOMEPAGE_H
#define HOMEPAGE_H

// Everything up to and including the opening <div id="sensorContainer">
// Sensor cards are injected between Part 1 and Part 2 at runtime
const char homePagePart1[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Sensor Dashboard</title>
  <meta name="viewport" content="width=device-width, initial-scale=1"> <!-- Mobile friendly -->
  <meta http-equiv="refresh" content="30"> <!-- Fallback full page refresh every 30s -->
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
    h1 { text-align: center; margin-top: 20px; }

    /* Responsive card grid */
    .container {
      display: flex;
      flex-wrap: wrap;
      justify-content: center;
      margin: 20px;
    }

    /* Individual sensor card */
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
    .value { font-size: 1.5em; font-weight: bold; margin-top: 5px; }

    /* Green banner showing ThingSpeak upload status */
    .status {
      background-color: #4CAF50;
      color: white;
      padding: 10px;
      border-radius: 5px;
      margin: 10px;
      font-size: 0.9em;
    }
  </style>
</head>
<body>
  <h1>ESP32 Sensor Dashboard</h1>
  <div class="status">Data streaming to ThingSpeak every 20s</div>
  <div class="container" id="sensorContainer"> <!-- Cards injected here -->
)rawliteral";

// Closing tags + JavaScript that polls /sensors every 2 seconds
// and updates the card container without reloading the full page
const char homePagePart2[] = R"rawliteral(
  </div>

  <script>
    // Fetches only the sensor cards from the ESP32 and swaps them in
    function updateSensors() {
      fetch('/sensors')
        .then(response => response.text())
        .then(data => {
          document.getElementById('sensorContainer').innerHTML = data;
        })
        .catch(err => console.error(err));
    }
    setInterval(updateSensors, 2000); // Refresh cards every 2 seconds
    window.onload = updateSensors;    // Also run immediately on page load
  </script>
</body>
</html>
)rawliteral";

#endif