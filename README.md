# MiniBulldozer
Long story short. I wanted to build a mini version of a bulldozer to clean gutters. It is a little more waterproff than a bucket. It can cross muddy and watery terrain, as long as it's not too deep ~5cm.
Components used:
  - 2 Arduino Nano
  - 2 Electic motors with 46.8 reduction ratio
  - L298N Driver Controller
  - NRF24L01 Wireless Transreceiver.
  - 2 PS2 Dual-axis controller.
The carcass was 3D printed while the rest of the plastic lego parts were involuntarily donated by the younger members of the family.
Here below is rough schematic of project and a few photos of the robot itself. (The schematic doesn't include the 12v and 5v power supplies) 
![image](https://github.com/user-attachments/assets/747d3722-6b7e-41db-b6bb-a8c4a51d8cdd)
![20241214_101152](https://github.com/user-attachments/assets/b6a7dcae-883b-4b99-93f0-a150277b0177)
![20241214_101216](https://github.com/user-attachments/assets/2ea1559a-fc46-483f-bb72-4842ba74b075)

# Motors
The main file that receives the signal from the antenna and calculates the direction and speed for the dozer based on the inputs from the operator.

# Transimitter
The input from the Dual-Axis controller are being fed into the arduino nano and then converted sent to NRL24L01 which then transmits it to bulldozer (The "Motors" file). One controller is responsible for the dozer's movement and the other is for moving the bucket. 
