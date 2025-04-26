from flask import Flask, request, jsonify, render_template
import json
from datetime import datetime

app = Flask(__name__)

# File lưu dữ liệu sensor gửi lên
DATA_FILE = 'data.json'
# File lưu lệnh điều khiển mái che
COMMAND_FILE = 'command.json'

@app.route('/')
def index():
    return render_template('index.html')

# API nhận dữ liệu từ ESP32 gửi lên (POST)
@app.route('/data', methods=['POST'])
def receive_data():
    data = request.json
    data['timestamp'] = datetime.now().strftime("%Y-%m-%d %H:%M:%S")  # Ghi thêm timestamp

    # Ghi dữ liệu nhận được vào file data.json
    with open(DATA_FILE, 'a') as f:
        f.write(json.dumps(data) + '\n')

    print(">> Received from ESP32:", data)
    return jsonify({'status': 'success'}), 200

# API lấy dữ liệu ra để hiển thị (GET)
@app.route('/get-data', methods=['GET'])
def get_data():
    try:
        with open(DATA_FILE, 'r') as f:
            lines = f.readlines()
            data = [json.loads(line) for line in lines[-20:]]  # Lấy 20 dòng gần nhất
        return jsonify(data)
    except Exception as e:
        return jsonify({'error': str(e)})

# API nhận lệnh điều khiển mái che (POST)
@app.route('/control', methods=['POST'])
def control_roof():
    command = request.json.get('command')

    # Lưu lệnh vào file command.json
    with open(COMMAND_FILE, 'w') as f:
        json.dump({'command': command}, f)
    
    # Trả về trạng thái thành công
    return jsonify({'status': 'command_sent', 'command': command})

# Chạy server Flask
if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
