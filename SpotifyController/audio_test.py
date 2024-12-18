import unittest
from unittest.mock import patch, MagicMock
import sys
import os
sys.path.append(os.path.abspath(os.path.dirname(__file__)))
from audio import AudioController
"""
This class represents our mock test cases by simulating the behavior of serial ports and 
subprocesses for audio.py which handles our volume manipulation via the potentiometer. 
"""
class TestAudioController(unittest.TestCase):
    def setUp(self):
        self.serial_port = "COM_TEST"
        self.baud_rate = 115200
        self.threshold = 10
        self.audio_controller = AudioController(self.serial_port, self.baud_rate, self.threshold)

    @patch("subprocess.run")
    def test_set_volume(self, mock_subprocess):
        self.audio_controller.set_volume(0.5)  
        mock_subprocess.assert_called_once_with(
            ["osascript", "-e", "set volume output volume 50"], check=True
        )
        self.audio_controller.set_volume(0.0)
        mock_subprocess.assert_called_with(
            ["osascript", "-e", "set volume output volume 0"], check=True
        )

    def test_clean_data(self):
        self.assertEqual(self.audio_controller.clean_data("123"), "123")
        self.assertEqual(self.audio_controller.clean_data("12abc34"), "1234")
        self.assertEqual(self.audio_controller.clean_data("!@#$"), "")

    @patch('audio.AudioController.set_volume')
    @patch('serial.Serial')
    def test_run_with_valid_data(self, mock_serial, mock_set_volume):
        mock_serial_instance = mock_serial.return_value
        mock_serial_instance.readline.side_effect = [
            b'500\n',  
            b'1023\n',
            b'0\n',   
            b''        
        ]

        controller = AudioController(serial_port="dummy", threshold=0)
        controller.run(max_iterations=3)
        expected_calls = [500 / 1023.0, 1.0, 0.0]
        actual_calls = [call[0][0] for call in mock_set_volume.call_args_list]
        print("Actual calls to set_volume:", actual_calls)  

        for actual, expected in zip(actual_calls, expected_calls):
            self.assertAlmostEqual(actual, expected, places=2)

    @patch("serial.Serial")
    def test_run_with_invalid_data(self, mock_serial):
        mock_serial_instance = MagicMock()
        mock_serial_instance.readline.side_effect = [
            b"abc\n",  
            b"1200\n", 
            b"-50\n",  
            b"",       
        ]
        mock_serial.return_value = mock_serial_instance
        with patch.object(self.audio_controller, "set_volume") as mock_set_volume:
            self.audio_controller.running = False  
            self.audio_controller.run()
            mock_set_volume.assert_not_called()

    def test_stop(self):
        self.audio_controller.running = True
        self.audio_controller.stop()
        self.assertFalse(self.audio_controller.running)


if __name__ == "__main__":
    unittest.main()