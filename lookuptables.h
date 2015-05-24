#ifndef LOOKUPTABLES_H
#define LOOKUPTABLES_H


const char* hex_strings[] = {
"0x00", "0x01", "0x02", "0x03", "0x04", "0x05", "0x06", "0x07", "0x08", "0x09", 
"0x0A", "0x0B", "0x0C", "0x0D", "0x0E", "0x0F", "0x10", "0x11", "0x12", "0x13", 
"0x14", "0x15", "0x16", "0x17", "0x18", "0x19", "0x1A", "0x1B", "0x1C", "0x1D", 
"0x1E", "0x1F", "0x20", "0x21", "0x22", "0x23", "0x24", "0x25", "0x26", "0x27", 
"0x28", "0x29", "0x2A", "0x2B", "0x2C", "0x2D", "0x2E", "0x2F", "0x30", "0x31", 
"0x32", "0x33", "0x34", "0x35", "0x36", "0x37", "0x38", "0x39", "0x3A", "0x3B", 
"0x3C", "0x3D", "0x3E", "0x3F", "0x40", "0x41", "0x42", "0x43", "0x44", "0x45", 
"0x46", "0x47", "0x48", "0x49", "0x4A", "0x4B", "0x4C", "0x4D", "0x4E", "0x4F", 
"0x50", "0x51", "0x52", "0x53", "0x54", "0x55", "0x56", "0x57", "0x58", "0x59", 
"0x5A", "0x5B", "0x5C", "0x5D", "0x5E", "0x5F", "0x60", "0x61", "0x62", "0x63", 
"0x64", "0x65", "0x66", "0x67", "0x68", "0x69", "0x6A", "0x6B", "0x6C", "0x6D", 
"0x6E", "0x6F", "0x70", "0x71", "0x72", "0x73", "0x74", "0x75", "0x76", "0x77", 
"0x78", "0x79", "0x7A", "0x7B", "0x7C", "0x7D", "0x7E", "0x7F", "0x80", "0x81", 
"0x82", "0x83", "0x84", "0x85", "0x86", "0x87", "0x88", "0x89", "0x8A", "0x8B", 
"0x8C", "0x8D", "0x8E", "0x8F", "0x90", "0x91", "0x92", "0x93", "0x94", "0x95", 
"0x96", "0x97", "0x98", "0x99", "0x9A", "0x9B", "0x9C", "0x9D", "0x9E", "0x9F", 
"0xA0", "0xA1", "0xA2", "0xA3", "0xA4", "0xA5", "0xA6", "0xA7", "0xA8", "0xA9", 
"0xAA", "0xAB", "0xAC", "0xAD", "0xAE", "0xAF", "0xB0", "0xB1", "0xB2", "0xB3", 
"0xB4", "0xB5", "0xB6", "0xB7", "0xB8", "0xB9", "0xBA", "0xBB", "0xBC", "0xBD", 
"0xBE", "0xBF", "0xC0", "0xC1", "0xC2", "0xC3", "0xC4", "0xC5", "0xC6", "0xC7", 
"0xC8", "0xC9", "0xCA", "0xCB", "0xCC", "0xCD", "0xCE", "0xCF", "0xD0", "0xD1", 
"0xD2", "0xD3", "0xD4", "0xD5", "0xD6", "0xD7", "0xD8", "0xD9", "0xDA", "0xDB", 
"0xDC", "0xDD", "0xDE", "0xDF", "0xE0", "0xE1", "0xE2", "0xE3", "0xE4", "0xE5", 
"0xE6", "0xE7", "0xE8", "0xE9", "0xEA", "0xEB", "0xEC", "0xED", "0xEE", "0xEF", 
"0xF0", "0xF1", "0xF2", "0xF3", "0xF4", "0xF5", "0xF6", "0xF7", "0xF8", "0xF9", 
"0xFA", "0xFB", "0xFC", "0xFD", "0xFE", "0xFF",
};

const char* bin_strings[] = {
"0b00000000", "0b00000001", "0b00000010", "0b00000011", "0b00000100", "0b00000101", "0b00000110", "0b00000111", "0b00001000", "0b00001001", 
"0b00001010", "0b00001011", "0b00001100", "0b00001101", "0b00001110", "0b00001111", "0b00010000", "0b00010001", "0b00010010", "0b00010011", 
"0b00010100", "0b00010101", "0b00010110", "0b00010111", "0b00011000", "0b00011001", "0b00011010", "0b00011011", "0b00011100", "0b00011101", 
"0b00011110", "0b00011111", "0b00100000", "0b00100001", "0b00100010", "0b00100011", "0b00100100", "0b00100101", "0b00100110", "0b00100111", 
"0b00101000", "0b00101001", "0b00101010", "0b00101011", "0b00101100", "0b00101101", "0b00101110", "0b00101111", "0b00110000", "0b00110001", 
"0b00110010", "0b00110011", "0b00110100", "0b00110101", "0b00110110", "0b00110111", "0b00111000", "0b00111001", "0b00111010", "0b00111011", 
"0b00111100", "0b00111101", "0b00111110", "0b00111111", "0b01000000", "0b01000001", "0b01000010", "0b01000011", "0b01000100", "0b01000101", 
"0b01000110", "0b01000111", "0b01001000", "0b01001001", "0b01001010", "0b01001011", "0b01001100", "0b01001101", "0b01001110", "0b01001111", 
"0b01010000", "0b01010001", "0b01010010", "0b01010011", "0b01010100", "0b01010101", "0b01010110", "0b01010111", "0b01011000", "0b01011001", 
"0b01011010", "0b01011011", "0b01011100", "0b01011101", "0b01011110", "0b01011111", "0b01100000", "0b01100001", "0b01100010", "0b01100011", 
"0b01100100", "0b01100101", "0b01100110", "0b01100111", "0b01101000", "0b01101001", "0b01101010", "0b01101011", "0b01101100", "0b01101101", 
"0b01101110", "0b01101111", "0b01110000", "0b01110001", "0b01110010", "0b01110011", "0b01110100", "0b01110101", "0b01110110", "0b01110111", 
"0b01111000", "0b01111001", "0b01111010", "0b01111011", "0b01111100", "0b01111101", "0b01111110", "0b01111111", "0b10000000", "0b10000001", 
"0b10000010", "0b10000011", "0b10000100", "0b10000101", "0b10000110", "0b10000111", "0b10001000", "0b10001001", "0b10001010", "0b10001011", 
"0b10001100", "0b10001101", "0b10001110", "0b10001111", "0b10010000", "0b10010001", "0b10010010", "0b10010011", "0b10010100", "0b10010101", 
"0b10010110", "0b10010111", "0b10011000", "0b10011001", "0b10011010", "0b10011011", "0b10011100", "0b10011101", "0b10011110", "0b10011111", 
"0b10100000", "0b10100001", "0b10100010", "0b10100011", "0b10100100", "0b10100101", "0b10100110", "0b10100111", "0b10101000", "0b10101001", 
"0b10101010", "0b10101011", "0b10101100", "0b10101101", "0b10101110", "0b10101111", "0b10110000", "0b10110001", "0b10110010", "0b10110011", 
"0b10110100", "0b10110101", "0b10110110", "0b10110111", "0b10111000", "0b10111001", "0b10111010", "0b10111011", "0b10111100", "0b10111101", 
"0b10111110", "0b10111111", "0b11000000", "0b11000001", "0b11000010", "0b11000011", "0b11000100", "0b11000101", "0b11000110", "0b11000111", 
"0b11001000", "0b11001001", "0b11001010", "0b11001011", "0b11001100", "0b11001101", "0b11001110", "0b11001111", "0b11010000", "0b11010001", 
"0b11010010", "0b11010011", "0b11010100", "0b11010101", "0b11010110", "0b11010111", "0b11011000", "0b11011001", "0b11011010", "0b11011011", 
"0b11011100", "0b11011101", "0b11011110", "0b11011111", "0b11100000", "0b11100001", "0b11100010", "0b11100011", "0b11100100", "0b11100101", 
"0b11100110", "0b11100111", "0b11101000", "0b11101001", "0b11101010", "0b11101011", "0b11101100", "0b11101101", "0b11101110", "0b11101111", 
"0b11110000", "0b11110001", "0b11110010", "0b11110011", "0b11110100", "0b11110101", "0b11110110", "0b11110111", "0b11111000", "0b11111001", 
"0b11111010", "0b11111011", "0b11111100", "0b11111101", "0b11111110", "0b11111111",
};

#endif