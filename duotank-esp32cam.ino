#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <Preferences.h>
#include "esp_camera.h"

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);
WiFiServer streamServer(81);
Preferences prefs;

const int FLASH_LED = 4;
const int PAN_PIN = 12;
const int TILT_PIN = 13;

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

Servo panServo;
Servo tiltServo;

int leftTrack = 0;
int rightTrack = 0;
int panAngle = 126;
int tiltAngle = 90;

const char HTML_INDEX[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>Tank Cam Controller</title>
    <style>
        body { font-family: Arial, sans-serif; background: #121212; color: #fff; text-align: center; margin: 0; padding: 15px; user-select: none; }
        h2 { color: #00adb5; margin-bottom: 15px; }
        .btn-group { display: flex; justify-content: center; gap: 15px; max-width: 600px; margin: 0 auto 20px auto; }
        .btn { background: #393e46; border: 2px solid #00adb5; color: #fff; padding: 12px 25px; font-size: 14px; font-weight: bold; border-radius: 8px; cursor: pointer; transition: 0.2s; flex: 1; }
        .btn.active { background: #00adb5; color: #222831; }
        .btn-launch { background: #d9534f; border: 2px solid #ff6b6b; color: #fff; padding: 18px 40px; font-size: 18px; font-weight: bold; border-radius: 12px; cursor: pointer; box-shadow: 0 4px 15px rgba(217,83,79,0.4); transition: 0.1s; margin-top: 15px; width: 320px; }
        .btn-launch:dark { background: #c9302c; }
        .main-layout { display: flex; align-items: center; justify-content: center; gap: 40px; max-width: 950px; margin: 0 auto; }
        .track-box { background: #222831; padding: 20px; border-radius: 16px; height: 380px; display: flex; flex-direction: column; align-items: center; justify-content: space-between; width: 140px; box-shadow: 0 4px 10px rgba(0,0,0,0.3); }
        .stream-area { display: flex; flex-direction: column; align-items: center; gap: 10px; }
        .stream-container { background: #000; border: 3px solid #393e46; border-radius: 12px; width: 320px; height: 240px; display: flex; align-items: center; justify-content: center; overflow: hidden; position: relative; }
        .stream-container img { width: 100%; height: 100%; object-fit: cover; }
        .cam-controls { max-width: 600px; margin: 25px auto 0 auto; background: #222831; padding: 15px; border-radius: 12px; }
        .slider-title { font-weight: bold; margin-bottom: 5px; color: #eeeeee; display: flex; justify-content: space-between; }
        input[type=range] { -webkit-appearance: none; background: #393e46; border-radius: 8px; outline: none; width: 100%; height: 16px; }
        .track-box input[type=range] { transform: rotate(-90deg); width: 280px; height: 24px; margin-top: 130px; margin-bottom: 130px; background: #2d3238; }
        input[type=range]::-webkit-slider-thumb { -webkit-appearance: none; width: 44px; height: 44px; border-radius: 50%; background: #00adb5; cursor: pointer; box-shadow: 0 2px 6px rgba(0,0,0,0.5); }
        .cam-controls input[type=range]::-webkit-slider-thumb { width: 28px; height: 28px; }
    </style>
</head>
<body>
    <h2>Tank Control Panel</h2>
    <div class="btn-group">
        <button id="flashBtn" class="btn" onclick="toggleFlash()">&#128262; Flashlight: OFF</button>
        <button class="btn" onclick="triggerCenter()">&#127919; RECENTER CAM</button>
    </div>
    <div class="main-layout">
        <div class="track-box">
            <span style="font-size:16px; font-weight:bold; color:#00adb5;">LEFT</span>
            <span id="leftVal" style="font-size:20px; font-weight:bold;">0</span>
            <input type="range" id="leftSlider" min="-255" max="255" value="0">
        </div>
        <div class="stream-area">
            <div class="stream-container">
                <img id="cameraStream" src="" alt="Awaiting Stream...">
            </div>
            <button id="launchBtn" class="btn-launch">&#128640; LAUNCH DISC</button>
        </div>
        <div class="track-box">
            <span style="font-size:16px; font-weight:bold; color:#00adb5;">RIGHT</span>
            <span id="rightVal" style="font-size:20px; font-weight:bold;">0</span>
            <input type="range" id="rightSlider" min="-255" max="255" value="0">
        </div>
    </div>
    <div class="cam-controls">
        <div style="margin-bottom: 15px;">
            <div class="slider-title"><span>Pan</span><span id="panVal">126&deg;</span></div>
            <input type="range" id="panSlider" min="0" max="180" value="126">
        </div>
        <div>
            <div class="slider-title"><span>Tilt</span><span id="tiltVal">90&deg;</span></div>
            <input type="range" id="tiltSlider" min="40" max="130" value="90">
        </div>
    </div>
    <script>
        const leftSlider  = document.getElementById('leftSlider');
        const rightSlider = document.getElementById('rightSlider');
        const panSlider   = document.getElementById('panSlider');
        const tiltSlider  = document.getElementById('tiltSlider');
        const streamImg   = document.getElementById('cameraStream');
        const launchBtn   = document.getElementById('launchBtn');
        let flashOn = false;
        let lastSend = 0;
        const throttleDelay = 50;

        window.addEventListener('DOMContentLoaded', () => {
            setTimeout(() => {
                streamImg.src = window.location.protocol + "//" + window.location.hostname + ":81/stream";
            }, 600);
            fetch('/getangles')
            .then(res => res.json())
            .then(data => {
                panSlider.value = data.pan;
                tiltSlider.value = data.tilt;
                document.getElementById('panVal').innerHTML = data.pan + "&deg;";
                document.getElementById('tiltVal').innerHTML = data.tilt + "&deg;";
            }).catch(() => {});
        });

        function sendUpdate() {
            const now = Date.now();
            if (now - lastSend > throttleDelay) {
                const url = `/control?left=${leftSlider.value}&right=${rightSlider.value}&pan=${panSlider.value}&tilt=${tiltSlider.value}`;
                navigator.sendBeacon(url);
                lastSend = now;
            }
        }

        function triggerCenter() {
            panSlider.value = 126;
            tiltSlider.value = 90;
            document.getElementById('panVal').innerHTML = "126&deg;";
            document.getElementById('tiltVal').innerHTML = "90&deg;";
            const url = `/control?left=${leftSlider.value}&right=${rightSlider.value}&pan=126&tilt=90&save=1`;
            navigator.sendBeacon(url);
        }

        function startFiring() { navigator.sendBeacon('/launch?state=1'); }
        function stopFiring() { navigator.sendBeacon('/launch?state=0'); }

        launchBtn.addEventListener('pointerdown', startFiring);
        launchBtn.addEventListener('touchstart', (e) => { e.preventDefault(); startFiring(); });
        launchBtn.addEventListener('pointerup', stopFiring);
        launchBtn.addEventListener('touchend', stopFiring);
        launchBtn.addEventListener('pointerleave', stopFiring);

        function toggleFlash() {
            flashOn = !flashOn;
            const btn = document.getElementById('flashBtn');
            btn.innerHTML = flashOn ? "&#128262; Flashlight: ON" : "&#128262; Flashlight: OFF";
            btn.classList.toggle('active', flashOn);
            navigator.sendBeacon(`/flash?state=${flashOn ? 1 : 0}`);
        }

        panSlider.addEventListener('input', () => { document.getElementById('panVal').innerHTML = panSlider.value + "&deg;"; sendUpdate(); });
        tiltSlider.addEventListener('input', () => { document.getElementById('tiltVal').innerHTML = tiltSlider.value + "&deg;"; sendUpdate(); });
        
        panSlider.addEventListener('change', () => { navigator.sendBeacon(`/control?left=${leftSlider.value}&right=${rightSlider.value}&pan=${panSlider.value}&tilt=${tiltSlider.value}&save=1`); });
        tiltSlider.addEventListener('change', () => { navigator.sendBeacon(`/control?left=${leftSlider.value}&right=${rightSlider.value}&pan=${panSlider.value}&tilt=${tiltSlider.value}&save=1`); });

        leftSlider.addEventListener('input', () => { document.getElementById('leftVal').innerText = leftSlider.value; sendUpdate(); });
        rightSlider.addEventListener('input', () => { document.getElementById('rightVal').innerText = rightSlider.value; sendUpdate(); });

        function resetLeftTrack()  { leftSlider.value = 0;  document.getElementById('leftVal').innerText = 0;  forceFlush(); }
        function resetRightTrack() { rightSlider.value = 0; document.getElementById('rightVal').innerText = 0; forceFlush(); }
        function forceFlush() { navigator.sendBeacon(`/control?left=${leftSlider.value}&right=${rightSlider.value}&pan=${panSlider.value}&tilt=${tiltSlider.value}`); }

        leftSlider.addEventListener('pointerup', resetLeftTrack);   leftSlider.addEventListener('touchend', resetLeftTrack);
        rightSlider.addEventListener('pointerup', resetRightTrack); rightSlider.addEventListener('touchend', resetRightTrack);
    </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html; charset=utf-8", HTML_INDEX);
}

void handleControl() {
  if (server.hasArg("left")) leftTrack = server.arg("left").toInt();
  if (server.hasArg("right")) rightTrack = server.arg("right").toInt();
  if (server.hasArg("pan")) panAngle = server.arg("pan").toInt();
  if (server.hasArg("tilt")) tiltAngle = server.arg("tilt").toInt();
  int clippedTilt = constrain(tiltAngle, 40, 130);
  panServo.write(180 - panAngle);
  tiltServo.write(180 - clippedTilt);
  if (server.hasArg("save")) {
    prefs.begin("servo-pack", false);
    prefs.putInt("pan", panAngle);
    prefs.putInt("tilt", tiltAngle);
    prefs.end();
  }
  Serial.printf("touno:%d,%d\n", leftTrack, rightTrack);
  server.send(200, "text/plain; charset=utf-8", "OK");
}
void handleGetAngles() {
  prefs.begin("servo-pack", true);
  int savedPan = prefs.getInt("pan", 126);
  int savedTilt = prefs.getInt("tilt", 90);
  prefs.end();
  
  String json = "{\"pan\":" + String(savedPan) + ",\"tilt\":" + String(savedTilt) + "}";
  server.send(200, "application/json", json);
}

void handleLaunch() {
  if (server.hasArg("state")) {
    int state = server.arg("state").toInt();
    if (state == 1) {
      Serial.println("touno:FIRE_ON");
    } else {
      Serial.println("touno:FIRE_OFF");
    }
  }
  server.send(200, "text/plain; charset=utf-8", "OK");
}
void handleFlash() {
  if (server.hasArg("state")) {
    int state = server.arg("state").toInt();
    digitalWrite(FLASH_LED, state == 1 ? HIGH : LOW);
  }
  server.send(200, "text/plain; charset=utf-8", "OK");
}
void streamTask(void* pvParameters) {
  streamServer.begin();
  while (1) {
    WiFiClient client = streamServer.available();
    if (client) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: multipart/x-mixed-replace; boundary=123456789000000000000987654321");
      client.println();
      while (client.connected()) {
        camera_fb_t* fb = esp_camera_fb_get();
        if (!fb) break;
        client.println("--123456789000000000000987654321");
        client.println("Content-Type: image/jpeg");
        client.print("Content-Length: ");
        client.println(fb->len);
        client.println();
        client.write(fb->buf, fb->len);
        client.println();
        esp_camera_fb_return(fb);
        vTaskDelay(40 / portTICK_PERIOD_MS);
      }
      client.stop();
    }
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(FLASH_LED, OUTPUT);
  digitalWrite(FLASH_LED, LOW);
  prefs.begin("servo-pack", true);
  panAngle = prefs.getInt("pan", 126);
  tiltAngle = prefs.getInt("tilt", 90);
  prefs.end();
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  panServo.setPeriodHertz(50);
  tiltServo.setPeriodHertz(50);
  panServo.attach(PAN_PIN, 500, 2400);
  tiltServo.attach(TILT_PIN, 500, 2400);
  int clippedTilt = constrain(tiltAngle, 40, 130);
  panServo.write(180 - panAngle);
  tiltServo.write(180 - clippedTilt);
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 8000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 18;
  config.fb_count = 2;
  config.grab_mode = CAMERA_GRAB_LATEST;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  esp_camera_init(&config);
  sensor_t* s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);
    s->set_hmirror(s, 0);
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.on("/getangles", handleGetAngles);
  server.on("/launch", handleLaunch);
  server.on("/flash", handleFlash);
  server.begin();
  xTaskCreatePinnedToCore(streamTask, "streamTask", 4096, NULL, 1, NULL, 0);
}
void loop() {
  server.handleClient();
}
