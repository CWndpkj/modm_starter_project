#include <core/board.hpp>
#include <cstdint>
#include <modm/architecture/interface/build_id.hpp>
#include <modm/debug/logger.hpp>
#include <modm/platform/core/peripherals.hpp>
#include <stdint.h>
#include <string>
// hdr_only_lib
#include <test/header1.h>

using Usart1 = BufferedUart<UsartHal1>;
modm::IODeviceWrapper<Usart1, modm::IOBuffer::BlockIfFull> device;

modm::log::Logger modm::log::debug(device);
modm::log::Logger modm::log::info(device);
modm::log::Logger modm::log::warning(device);
modm::log::Logger modm::log::error(device);

using namespace Board;
using namespace std::chrono_literals;

int main() {
  Board::initialize();

  Usart1::connect<Board::Uart1_tx::Tx, Board::Uart1_rx::Rx>();
  Usart1::initialize<Board::SystemClock, 115200_Bd>();
  auto hex_encode = [](const auto &begin, const auto &end) -> std::string {
    static constexpr std::array digits = {'0', '1', '2', '3', '4', '5',
                                          '6', '7', '8', '9', 'a', 'b',
                                          'c', 'd', 'e', 'f'};

    std::string str;
    str.resize(static_cast<std::size_t>((end - begin) * 2U));
    auto i = 0U;
    for (auto iter = begin; iter != end; iter++) {
      str[i++] = digits[(*iter & 0xf0) >> 4];
      str[i++] = digits[*iter & 0xf];
    }
    return str;
  };

  auto build = modm::build_id();
  MODM_LOG_INFO << hex_encode(build.begin(), build.end()).c_str() << "\n\r";

  MODM_LOG_INFO << uint32_t(20);
  LedGreen::set();

  while (true) {
    LedGreen::toggle();
    modm::delay(Button::read() ? 100ms : 500ms);
  }

  return 0;
}
