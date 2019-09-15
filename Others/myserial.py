import serial

# python -m serial.tools.list_ports 





#ser.write(bytes('Soy Felipe\n','utf-8'))

#ser.close

ser = serial.Serial('COM6')
print(ser.name)

sample_ticket = bytes.fromhex('20202020202020202057414c4d415254204445204d455849434f0a0a0a4172746963756c6f732020202020202020202020202020202050726563696f0a0a4d616e7a616e617320202020202020202020202020202020202434350a636572766563697461732020202020202020202020202020202436350a67616e7369746f206d6172696e656c612020202020202020202431330a43657265616c202020202020202020202020202020202020202433320a0a4772616369617320706f7220737520636f6d707261203a290a1D564103');

tp_ESC = bytes.fromhex('1b')
tp_GS = bytes.fromhex('1d')
tp_LF = bytes.fromhex('0a')
tp_init = bytes.fromhex('1b40')
tp_feed_cut = bytes.fromhex('1d564103')



def init_printer():
	ser.write(tp_init);

def print_str(s):
	ser.write(bytes(s+'\n', 'utf-8'))

def end():
	ser.write(tp_feed_cut)




init_printer()
print_str('Hola soy felipe desde python')
end()
ser.write(sample_ticket)
ser.close()


