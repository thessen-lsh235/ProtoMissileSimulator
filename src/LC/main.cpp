#include "client/lc_client.h"
#include "server/lc_server.h"
#include "serial/lc_serial.h"
#include "logger/lc_logger.h"
#include "../common/target.h"
#include "../common/missile.h"

#include <iostream>
#include <thread>
#include <chrono>

int main()
{
    try
    {
        // 1. Logger 초기화
        LcLogger logger;
        if (!logger.startLogging("lc_log.txt")) {
            std::cerr << "[ERROR] 로그 파일 열기 실패" << std::endl;
            return -1;
        }
        logger.logMessage("LC 프로그램 시작");

        // 2. 하드코딩 설정
        std::string radarIp = "127.0.0.1";
        int radarPort = 5001;
        std::string serialDevice = "/dev/ttyUSB0";
        int baudrate = 9600;

        logger.logMessage("설정 값 하드코딩 완료");

        // 3. Client(Radar 접속)
        LcClient radarClient(radarIp, radarPort);
        if (!radarClient.connectToServer())
        {
            logger.logMessage("[ERROR] 레이더 서버 연결 실패");
            return -1;
        }
        logger.logMessage("레이더 서버 연결 성공");

        // 4. Serial(Launcher 연결)
        LcSerial launcherSerial(serialDevice, baudrate);
        if (!launcherSerial.openSerial())
        {
            logger.logMessage("[WARNING] 발사대 시리얼 연결 실패 - 무시하고 진행");
            // 실패해도 return 하지 않고 그냥 진행
        }
        else
        {
            logger.logMessage("발사대 시리얼 연결 성공");
        }

        // 5. Server(ECC 접속 대기)
        LcServer eccServer(5000);
        if (!eccServer.startServer())
        {
            logger.logMessage("[ERROR] ECC 서버 시작 실패");
            return -1;
        }
        logger.logMessage("ECC 서버 시작 완료");

        // 6. 메인 루프
        logger.logMessage("메인 루프 진입");

        while (true)
        {
            std::string eccCommand;
            std::cout << "명령 입력 (TURN_LEFT / TURN_RIGHT) > ";
            std::getline(std::cin, eccCommand);
        
            if (!eccCommand.empty())
            {
                logger.logMessage("콘솔로부터 명령 수신: " + eccCommand);
        
                // [예시] 수신한 명령으로 가짜 target 객체 생성
                TargetInfo target;
                target.id = 1;
                target.pos_x = 100.0;
                target.pos_y = 200.0;
                target.speed = 300;
                target.degree = 90.0;
        
                if (eccCommand == "TURN_LEFT")
                {
                    target.degree -= 10;
                    logger.logMessage("레이더 왼쪽으로 회전: " + std::to_string(target.degree));
                }
                else if (eccCommand == "TURN_RIGHT")
                {
                    target.degree += 10;
                    logger.logMessage("레이더 오른쪽으로 회전: " + std::to_string(target.degree));
                }
                else
                {
                    logger.logMessage("[WARNING] 알 수 없는 명령어: " + eccCommand);
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[FATAL] 예외 발생: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
