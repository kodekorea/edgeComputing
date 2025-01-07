import serial
import csv

def log_serial_to_csv(port, baudrate, output_file):
    try:
        # 시리얼 포트 열기
        ser = serial.Serial(port, baudrate, timeout=1)
        print(f"Connected to {port} at {baudrate} baud.")

        # CSV 파일 열기
        with open("data/"+output_file, mode='w', newline='') as csvfile:
            csv_writer = csv.writer(csvfile)

            # 헤더 추가
            csv_writer.writerow(["timestamp", "gyrox", "gyroy", "gyroz", "accx", "accy", "accz"])

            print("Logging data to CSV. Press Ctrl+C to stop.")

            timestamp = 10  # 초기 타임스탬프 값

            while True:
                try:
                    # 시리얼 데이터 읽기
                    if ser.in_waiting > 0:
                        data = ser.readline().decode('utf-8').strip()
                        data_split = data.split(",")  # 데이터를 쉼표로 분리

                        # 데이터 기록
                        if len(data_split) == 6:  # 데이터가 6개일 경우만 기록
                            csv_writer.writerow([timestamp] + data_split)
                            timestamp += 10  # 타임스탬프 증가

                        # 터미널 출력
                        print(f"{timestamp - 10}, {data}")
                except UnicodeDecodeError:
                    print("Error decoding data. Skipping...")
    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")
    except KeyboardInterrupt:
        print("Stopped logging.")
    finally:
        # 시리얼 포트 닫기
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Serial port closed.")

# 사용자 설정
serial_port = 'COM4'  # Windows: 'COM3', Linux/Mac: '/dev/ttyUSB0'
baud_rate = 115200
output_csv_file = 'test.csv'

log_serial_to_csv(serial_port, baud_rate, output_csv_file)
