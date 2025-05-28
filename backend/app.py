import logging
from flask import Flask, jsonify, request
import requests

logging.basicConfig(level=logging.INFO, format='%(asctime)s %(levelname)s %(message)s')
log = logging.getLogger()

app = Flask(__name__)

# socat → ESP32 (можно менять на прямой IP, если не нужен socat)
ESP32_URL = "http://host.docker.internal:8089/ring"

# оба эндпоинта будут обрабатывать один и тот же код
@app.route("/api/ring", methods=["POST"])
@app.route("/ring",     methods=["POST"])
def trigger_ring():
    try:
        # отправляем POST на ESP32
        response = requests.post(ESP32_URL, timeout=2)
        if response.ok:
            log.info("ESP32 rang successfully via POST")
            return jsonify({"status": "ok"}), 200
        else:
            log.warning(f"ESP32 failed, status {response.status_code}")
            return jsonify({"status": "fail"}), 500
    except Exception as e:
        log.error(f"Error calling ESP32: {e}")
        return jsonify({"status": "error", "detail": str(e)}), 500

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8084, debug=False, use_reloader=False, threaded=True)