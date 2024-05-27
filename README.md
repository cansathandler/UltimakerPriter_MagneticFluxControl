# UltimakerPriter_MagneticFluxControl

Purpose
- Create magnetic flux mapping.

Algorism
- Do scan

Preparation
- Install Anaconda or Python text editor with Jupitar notebook extension (e.g. Vistual Studio Code or Cursor)
- Download NI-VISA (https://www.ni.com/en/support/downloads/drivers/download.ni-visa.html#521671)
- Install 
    pyserial (in command prompt: type "pip install pyserial" or "conda install pyserial")
    pyvisa
    ipywidgets

## How to use

Hardware:
- Connect all the instruments to your laptop (Flux gauge: USB-mini, DC power supply, USB-A, Teensy4.0: USB-micro)

Software:
- proceed the program (shift + Enter) with modifying few variables according to the comment
- Adjust sensor position that you want to start
- Enter mapping dimension (x_mesh_size x y_mesh_size)