#include "can_message_dump.hpp"

std::unordered_map<uint8_t, std::string> message_colorization {
    {0x0, "red"},
    {0x1, "yellow"},
    {0x2, "yellow"},
    {0x3, "yellow"},
    {0x4, "green"},
    {0x5, "green"},
    {0x6, "green"},
    {0x7, "green"},
    {0x8, "green"},
    {0x9, "magenta"},
    {0xa, "blue"},
    {0xb, "blue"},
    {0xc, "cyan"},
    {0xd, "magenta"},
    {0xe, "purple"},
    {0xf, "white"},
};

int Open_socket(const char *if_name){
    int socket_id;

    struct sockaddr_can addr;
    struct ifreq ifr;

    // ceeate and bind socket
    if ((socket_id = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Socket");
        return 1;
    }

    std::strcpy(ifr.ifr_name, if_name);
    ioctl(socket_id, SIOCGIFINDEX, &ifr);
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_id, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("Bind");
        return 1;
    }

    return socket_id;
}

void Print_header(){
    std::cout << "\033[s";       // save cursor position
    std::cout << "\033[H\033[K"; // move to top left
    std::string header_line = emio::format("{:^12} {:^8} {:^32} {:^20} {:^20} {:^7}   {:}", "Time", "ID", "Message", "Module", "Instance", "Length", "Data");
    std::cout << header_line;
    std::cout << "\033[u";       // restore cursor position
    std::cout << std::flush;
}

void Prepare_terminal(){
    std::cout << "\033[2J";      // clear the terminal
    std::cout << "\033[H\033[K"; // move to top left
    std::cout << "\033[B";       // move one down, make space for header
}

int Read_frame(int socket, struct can_frame *frame){
    int nbytes = read(socket, frame, sizeof(struct can_frame));

    if (nbytes < 0) {
        perror("Read");
        return 1;
    }

    if (nbytes < static_cast<int>(sizeof(struct can_frame))) {
        std::cerr << "Read: incomplete CAN frame" << std::endl;
        return 1;
    }

    return nbytes;
}

void Print_app_frame(int time_ms, struct can_frame *frame){
    std::string time = emio::format("[{:09.3f}]", time_ms/1000.0);
    std::string id = emio::format("{:08x}", frame->can_id & 0x1FFFFFFF);
    std::string length = std::to_string(frame->can_dlc);
    uint16_t message_code = (frame->can_id >> 16) & 0xFFF;
    uint8_t module_code = (frame->can_id >> 4) & 0xFF;
    uint8_t instance_code = frame->can_id & 0xF;

    std::string message_type = std::string(magic_enum::enum_name(static_cast<Codes::Message_type>(message_code)));
    std::string message_colored = dye::colorize(message_colorization[(message_code & 0xf00) >> 8], message_type );
    std::string module = std::string(magic_enum::enum_name(static_cast<Codes::Module>(module_code)));
    std::string instance = std::string(magic_enum::enum_name(static_cast<Codes::Instance>(instance_code)));

    std::string data = "";

    for (int i = 0; i < frame->can_dlc; i++) {
        data += emio::format("0x{:02x} ", frame->data[i]);
    }

    // Align count with escape sequences for colors (9 characters for each colorized word)
    std::string frame_line = emio::format("{:^12} {:^8} {:<40} {:^20} {:^20} {:^7}   {:}", time, id, message_colored, module, instance ,length , data);

    std::cout << frame_line << std::endl;
}

void Print_admin_frame(int time_ms, struct can_frame *frame){
    std::string time = emio::format("[{:09.3f}]", time_ms/1000.0);
    std::string id = emio::format("{:03x}", frame->can_id & 0x7FF);
    std::string length = std::to_string(frame->can_dlc);

    std::string message_type = std::string(magic_enum::enum_name(static_cast<Codes::Command_admin>(frame->can_id & 0x7FF)));
    std::string data = "";

    for (int i = 0; i < frame->can_dlc; i++) {
        data += emio::format("0x{:02x} ", frame->data[i]);
    }

    std::string frame_line = emio::format("{:^12} {:>8} {:^20} {:^33} {:^7}   {:}", time, id, message_type, "",length , data);

    std::cout << frame_line << std::endl;
}

bool Check_interface_exists(const char* interface_name) {
    struct ifreq ifr;
    int sock = socket(AF_CAN, SOCK_RAW, CAN_RAW);

    if (sock < 0) {
        return false;
    }

    std::strncpy(ifr.ifr_name, interface_name, IFNAMSIZ);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        close(sock);
        return false;
    }

    close(sock);
    return true;
}

int main(int argc, char **argv) {
    try {
        const char* interface = argc > 1 ? argv[1] : DEFAULT_CAN_INTERFACE;

        if (!Check_interface_exists(interface)) {
            std::cerr << "Usage: " << argv[0] << " <can_interface>" << std::endl;
            return 1;
        }

        auto start_time = std::chrono::steady_clock::now();
        int socket = Open_socket(interface);

        if (socket < 0) {
            return 1;
        }

        Prepare_terminal();
        Print_header();

        struct can_frame frame;

        while (true) {
            Read_frame(socket, &frame);
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::steady_clock::now() - start_time).count();

            if (frame.can_id & CAN_EFF_FLAG) {
                Print_app_frame(elapsed_ms, &frame);
            } else {
                Print_admin_frame(elapsed_ms, &frame);
            }

            Print_header();
        }

        close(socket);
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
}
