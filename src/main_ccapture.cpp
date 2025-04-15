#include <chrono>
#include <3dr/argparse.hpp>

#include <fcntl.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <3dr/camera.hpp>
#include <3dr/image.hpp>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev.h>

using namespace tdr;

static constexpr RealD DEFAULT_SPEED = 0.15;
static constexpr RealD DEFAULT_SPEED_ANGULAR = 0.02;

int main(int argc, char **argv) {
    ArgParser parse;
    std::optional<std::filesystem::path> evdev_path;
    parse.register_free([&] (const auto& cur) {
        evdev_path = cur;
    });
    parse(argc, argv);

    if (!evdev_path.has_value()) {
        std::cerr << argv[0] << ": input device path expected" << std::endl;
        return 1;
    }

    int fd = open(evdev_path.value().c_str(), O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("failed to open input device");
        return 1;
    }

    libevdev *dev = nullptr;
    if (libevdev_new_from_fd(fd, &dev) < 0) {
        perror("failed to initialize evdev");
        close(fd);
        return 1;
    }

    auto frame_delay_ms = static_cast<unsigned long>(static_cast<RealD>(1000) / DEFAULT_FRAMERATE);
    std::chrono::time_point<std::chrono::steady_clock> next_update = std::chrono::steady_clock::now();

    auto camera = Camera::default_for_viewport_size(DEFAULT_VIEWPORT_SIZE);

    while (true) {
        auto current_time = std::chrono::steady_clock::now();
        if (current_time >= next_update) {
            next_update = current_time + std::chrono::milliseconds(frame_delay_ms);
            std::cout << Json(camera) << std::endl;
        }

        input_event event { };
        int res = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &event);
        if (res == 0) {
            if (event.type == EV_KEY && event.value) {
                switch (event.code) {
                case KEY_W:
                    camera.move_along(camera.get_sight_direction() * DEFAULT_SPEED);
                    break;
                case KEY_S:
                    camera.move_along(camera.get_sight_direction() * -DEFAULT_SPEED);
                    break;
                case KEY_D:
                    camera.move_along(camera.get_rightwards_perpendicular() * DEFAULT_SPEED);
                    break;
                case KEY_A:
                    camera.move_along(camera.get_rightwards_perpendicular() * -DEFAULT_SPEED);
                    break;
                case KEY_UP:
                    camera.rotate_about(camera.get_rightwards_perpendicular(), -DEFAULT_SPEED_ANGULAR);
                    break;
                case KEY_DOWN:
                    camera.rotate_about(camera.get_rightwards_perpendicular(), DEFAULT_SPEED_ANGULAR);
                    break;
                case KEY_LEFT:
                    camera.rotate_about(camera.get_upwards_perpendicular(), -DEFAULT_SPEED_ANGULAR);
                    break;
                case KEY_RIGHT:
                    camera.rotate_about(camera.get_upwards_perpendicular(), DEFAULT_SPEED_ANGULAR);
                    break;
                default:
                }
            }
        } else if (res == -EAGAIN) {
            std::this_thread::sleep_until(next_update);
        } else if (res == LIBEVDEV_READ_STATUS_SYNC) {
            continue;
        } else {
            perror("end of event stream");
            break;
        }
    }

    libevdev_free(dev);
    close(fd);
}
