# UltimakerPriter_MagneticFluxControl

Purpose
- Create magnetic flux mapping.

Algorism
- shift coordinate point. For example, if there is 3x3 inspection area, The coordinate point would be 0,0 >> 0,1 >> 0,2 >> 0,3 >> 0,0 >> 0,1 >> 1,1 >> 1,2 >> 1,3 >> 0,1 >> 0,2 >> ... >> 3,3

Preparation
- Install Anaconda or Python text editor with Jupitar notebook extension (e.g. Vistual Studio Code or Cursor)
- Download NI-VISA (https://www.ni.com/en/support/downloads/drivers/download.ni-visa.html#521671)
- Install 
    pyserial (in command prompt: type "pip install pyserial" or "conda install pyserial")
    pyvisa
    ipywidgets
    Gaussmeter software: https://drive.google.com/drive/folders/1CqhIzMeMHimEZGLLzxYSF4xN3l3T1qgJ?usp=sharing


## How to use

Connection to instruments:
- Connect all the instruments to your laptop (Flux gauge: USB-mini, DC power supply, USB-A, Teensy4.0: USB-micro)

Software:
- Dowload the program (MagneticFluxMeasure.ipynb)
- proceed the program (shift + Enter) with modifying few variables according to the comment
- Adjust sensor position that you want to start
- Enter mapping dimension (x_mesh_size x y_mesh_size)

For your information:
- Source code for Teensy4.0 (Ultimaker_Printer_Serial_Move)