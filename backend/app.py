import logging
from flask import Flask, jsonify, request
import requests

logging.basicConfig(level=logging.INFO, format='%(asctime)s %(levelname)s %(message)s')
log = logging.getLogger()

app = Flask(__name__)

# host.docker.internal это адрес хоста (на 8089 порт пробрасывается ESP32)
ESP32_URL = "http://host.docker.internal:8089"

# оба эндпоинта будут обрабатывать один и тот же код
@app.route("/api/ring", methods=["POST"])
@app.route("/ring",     methods=["POST"])
def trigger_ring():
    try:
        # отправляем POST на ESP32
        response = requests.post(ESP32_URL+'/ring', timeout=5)

        if response.ok:
            data = response.json()
            return jsonify({
                "status": "ok",
                "duration": data.get('duration', 0),
                "melodyIndex": data.get('melodyIndex', 0)
            }), 200
        # if response.ok:
        #     log.info("ESP32 rang successfully via POST")
        #     return jsonify({"status": "ok"}), 200
        else:
            log.warning(f"ESP32 failed, status {response.status_code}")
            return jsonify({"status": "fail"}), 500
    except Exception as e:
        log.error(f"Error calling ESP32: {e}")
        return jsonify({"status": "error", "detail": str(e)}), 500


@app.route("/api/nextMelody", methods=["POST"])
@app.route("/nextMelody",     methods=["POST"])
def next_melody():
    try:
        response = requests.post(ESP32_URL+'/nextMelody', timeout=5)
        if response.ok:
            data = response.json()
            log.info(f"Мелодия изменена, текущая: {data.get('currentMelody', 'неизвестно')}")
            return jsonify({"status": "ok", "currentMelody": data.get('currentMelody')}), 200
        else:
            log.warning(f"Ошибка смены мелодии, статус {response.status_code}")
            return jsonify({"status": "fail"}), 500
    except Exception as e:
        log.error(f"Ошибка вызова ESP32: {e}")
        return jsonify({"status": "error", "detail": str(e)}), 500

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8084, debug=False, use_reloader=False, threaded=True)