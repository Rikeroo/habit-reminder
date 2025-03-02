import pytest
from pytest_embedded import Dut

@pytest.fixture(scope='module')
def neopixel(dut: Dut):
    dut.expect('NeoPixel test ready', timeout=10)
    yield dut
    dut.write('deinit')
    dut.expect('NeoPixel deinitialised', timeout = 5)

@pytest.fixture(scope='module', autouse=True)
def build_test_app(dut):
    dut.app.path = str(dut.template_path / 'pytest_main')
    dut.build()