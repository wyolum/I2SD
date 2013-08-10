
STARTING = 1
MSG_STATE = 2
STOPPING = 3

def toint(bits):
    out = 0
    for b in bits:
        out = out << 1
        out += b
    return out

class Msg_parser:
    def __init__(self):
        self.values = {}
        self.start_seq = [0,0,0,0,0,0,1,1,1,1,1,1,1,1]
        self.stop_seq = [1,1,1,1,1]
        self.start_int = toint(self.start_seq)
        self.stop_int = toint(self.stop_seq)
        self.stop_len = len(self.stop_seq)
        self.msg_len = 13
        self.buffer = 1
        self.state = STARTING
        self.count = 0
        self.msg = 0

    def on_bit(self, bit):
        self.buffer = ((self.buffer << 1) + b) % (1 << len(self.start_seq))
        self.count += 1
        if self.state == STARTING:
            if self.buffer == self.start_int:
                # print 'switching to MSG_STATE'
                self.buffer = 0
                self.count = 0
                self.state = MSG_STATE
        elif self.state == MSG_STATE:
            if self.count == self.msg_len:
                # print 'switching to STOPPING'
                self.msg = self.buffer
                self.state = STOPPING
                self.buffer = 0
                self.count = 0
        elif self.state == STOPPING:
            if self.count == self.stop_len:
                # print 'switching to STARTING'
                self.state = STARTING
                if self.buffer == self.stop_int:
                    print 'valid_msg', self.msg
                    self.buffer = 1                    
                else:
                    print 'invalid_msg'
                    self.buffer = 1
        return self.state
bits = '''110000000011111111011010001001011111110000000011111111101010000101011111110000000011111111001010001101011111000000001111111101110000100011111111000000001111111110110000010011111111000000001111111100110000110011111111000000001111111101010000101011111111000000001111111110010000011011111111000000001111111100010000111011111111000000001111111101100000100111111111000000001111111110100000010111111111000000001111111100100000110111111111110000000011111111100000000111111111'''
bits = [[0,1][b=='1'] for b in bits]
mp = Msg_parser()
for b in bits:
    state = mp.on_bit(b)
print '0x%04x' % mp.start_int
print '0x%04x' % mp.stop_int
here
class Msg_parser_old:
    def __init__(self):
        self.values = {}
        self.start_seq = [0,0,0,0,0,0,1,1,1,1,1,1,1,1]
        self.stop_seq = [1,1,1,1,1]
        self.state = STARTING
        self.msg_len = 13
        self.msg = []
        
    def parse_bitstream(self, bits):
        cursor = 0
        state = STARTING
        start_int = toint(self.start_seq)
        stop_int = toint(self.stop_seq)
        while len(bits) > 0:
            buffer = toint([1 for i in range(len(self.start_seq))])
            while len(bits) > 0 and buffer !=start_int:
                b = bits.pop(0)
                buffer = ((buffer << 1) + b) % (1 << len(self.start_seq))
                # print b, buffer, start_int
                # print buffer, b, buffer == start_int, start_int
            # print 'found start'
            msg = bits[:self.msg_len]
            bits = bits[self.msg_len:]
            # print toint(bits[:len(self.stop_seq)]), stop_int
            print 'msg:', ''.join(map(str, msg))
            if toint(bits[:len(self.stop_seq)]) == stop_int:
                bits = bits[len(self.stop_seq):]
            else:
                # print 'invalid', ''.join(map(str, bits[:6]))
                print 'invalid', toint(bits[:6]), stop_int, ''.join(map(str, bits[:6]))
                for b in msg[::-1]:
                    bits.insert(0, b)
                    
        
