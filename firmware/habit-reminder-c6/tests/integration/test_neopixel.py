import pytest
from pytest_embedded import Dut

@pytest.mark.esp32c6
@pytest.mark.parametrize('count', [3], indirect=True)
class TestNeoPixelIntegration:
    
    def test_led_sequence(self, neopixel):
        # tset LED cycling pattern
        for i in range(3):
            neopixel.write(f'set_let  {i} 255 0 0')
            neopixel.expect(f'LED {i} set to (255, 0, 0)', timeout=2)
            neopixel.write(f'get_led {i}')
            neopixel.expect(f'LED {i} color: (255, 0, 0)', timeout=2)