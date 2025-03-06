#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

static const uint8_t utf8d_PROGMEM[] PROGMEM = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 00..1f
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 20..3f
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 40..5f
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 60..7f
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,  // 80..9f
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,  // a0..bf
  8, 8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // c0..df
  0xa, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x3,                  // e0..ef
  0xb, 0x6, 0x6, 0x6, 0x5, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8,                  // f0..ff
  0x0, 0x1, 0x2, 0x3, 0x5, 0x8, 0x7, 0x1, 0x1, 0x1, 0x4, 0x6, 0x1, 0x1, 0x1, 0x1,                  // s0..s0
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1,  // s1..s2
  1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1,  // s3..s4
  1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 1,  // s5..s6
  1, 3, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // s7..s8
};

uint32_t decode(uint32_t* state, uint32_t* codep, uint32_t byte) {
  uint8_t utf8d;
  memcpy_P(&utf8d, &utf8d_PROGMEM[byte], sizeof utf8d);
  uint32_t type = utf8d;
  *codep = (*state != UTF8_ACCEPT) ? (byte & 0x3fu) | (*codep << 6) : (0xff >> type) & (byte);
  memcpy_P(&utf8d, &utf8d_PROGMEM[256 + *state * 16 + type], sizeof utf8d);
  *state = utf8d;
  return *state;
}



String printCodePoints(uint8_t* s) {
  uint32_t codepoint;
  uint32_t state = 0;
  String result;

  for (; *s; ++s) {
    if (decode(&state, &codepoint, *s))
      continue;
    if (codepoint <= 0xFFFF) {
      if (codepoint < 0x1000)
        result += "0";
      if (codepoint < 0x0100)
        result += "0";
      if (codepoint < 0x0010)
        result += "0";
      result += String(codepoint, HEX);
      continue;
    }
    // Encode code points above U+FFFF as surrogate pair.
    //    result += String((0xD7C0 + (codepoint >> 10)), HEX);
    //    result += String((0xDC00 + (codepoint & 0x3FF)), HEX);
  }

  if (state != UTF8_ACCEPT)
    result += "The string is not well-formed\n";

  return result;
}

// Converts a single Unicode code point to a UTF-8 string
String codePointToUTF8(uint32_t codepoint) {
  String result;
  if (codepoint <= 0x7F) {
    // 1-byte sequence
    result += (char)codepoint;
  } else if (codepoint <= 0x7FF) {
    // 2-byte sequence
    result += (char)(0xC0 | (codepoint >> 6));
    result += (char)(0x80 | (codepoint & 0x3F));
  } else if (codepoint <= 0xFFFF) {
    // 3-byte sequence
    result += (char)(0xE0 | (codepoint >> 12));
    result += (char)(0x80 | ((codepoint >> 6) & 0x3F));
    result += (char)(0x80 | (codepoint & 0x3F));
  } else if (codepoint <= 0x10FFFF) {
    // 4-byte sequence
    result += (char)(0xF0 | (codepoint >> 18));
    result += (char)(0x80 | ((codepoint >> 12) & 0x3F));
    result += (char)(0x80 | ((codepoint >> 6) & 0x3F));
    result += (char)(0x80 | (codepoint & 0x3F));
  } else {
    // Invalid code point
    return String();
  }
  return result;
}

// Converts a string of hexadecimal Unicode code points to a UTF-8 encoded string
String hexToUTF8(String hexStr) {
  String utf8String;

  for (size_t i = 0; i < hexStr.length(); i += 4) {
    String hexCode = hexStr.substring(i, i + 4);              // Extract 4 characters
    uint32_t codepoint = strtoul(hexCode.c_str(), NULL, 16);  // Convert hex to uint32_t
    utf8String += codePointToUTF8(codepoint);                 // Convert code point to UTF-8 and append
  }

  return utf8String;  // Return the final UTF-8 encoded string
}

String decodeUtf16(const char* hex) {
    // Calculate the length of the hex string
    size_t len = strlen(hex);

    // Check if length is even (for UTF-16)
    if (len % 4 != 0) {
        return "Invalid HEX string for UTF-16";
    }

    // Prepare a String object for the decoded UTF-16 string
    String utf16Str = "";

    // Convert hex to UTF-16
    for (size_t i = 0; i < len; i += 4) {
        // Extract four characters
        char hexPair[5] = {hex[i], hex[i + 1], hex[i + 2], hex[i + 3], '\0'};

        // Convert to integer
        int value;
        sscanf(hexPair, "%x", &value);

        // Append the character to the String (this assumes UTF-16LE)
        utf16Str += (char)(value & 0xFF);       // Low byte
        utf16Str += (char)(value >> 8);         // High byte
    }

    // Return the resulting UTF-16 string
    return utf16Str;
}

bool isValidUTF8(const uint8_t* data, size_t length) {
  size_t i = 0;

  while (i < length) {
    if (data[i] <= 0x7F) {
      // 1-byte character (0xxxxxxx)
      i++;
    } else if ((data[i] >= 0xC2) && (data[i] <= 0xDF)) {
      // 2-byte character (110xxxxx)
      if (i + 1 >= length || (data[i + 1] < 0x80 || data[i + 1] >= 0xC0)) {
        return false;  // Invalid
      }
      i += 2;
    } else if ((data[i] >= 0xE0) && (data[i] <= 0xEF)) {
      // 3-byte character (1110xxxx)
      if (i + 2 >= length || (data[i + 1] < 0x80 || data[i + 1] >= 0xC0) || (data[i + 2] < 0x80 || data[i + 2] >= 0xC0)) {
        return false;  // Invalid
      }
      i += 3;
    } else if ((data[i] >= 0xF0) && (data[i] <= 0xF4)) {
      // 4-byte character (11110xxx)
      if (i + 3 >= length || (data[i + 1] < 0x80 || data[i + 1] >= 0xC0) || (data[i + 2] < 0x80 || data[i + 2] >= 0xC0) || (data[i + 3] < 0x80 || data[i + 3] >= 0xC0)) {
        return false;  // Invalid
      }
      i += 4;
    } else {
      return false;  // Invalid byte
    }
  }

  return true;  // All bytes were valid
}

String identifyEncoding(const char* hex) {
  size_t len = strlen(hex);

  // Check if the hex string has an even number of digits
  if (len % 2 != 0) {
    return "Invalid HEX string.";
  }

  // Check for ASCII (only valid if characters are within 00 - 7F)
  bool isAscii = true;
  for (size_t i = 0; i < len; i += 2) {
    char hexPair[3] = { hex[i], hex[i + 1], '\0' };
    int value;
    sscanf(hexPair, "%x", &value);
    if (value < 0 || value > 127) {
      isAscii = false;
      break;
    }
  }

  // Check for UTF-16 (should have '00' before every character unless it's a low surrogate)
  bool isUtf16 = (len % 4 == 0);

  if (isUtf16) {
    for (size_t i = 0; i < len; i += 4) {
      char hexPair[5] = { hex[i], hex[i + 1], hex[i + 2], hex[i + 3], '\0' };
      int value;
      sscanf(hexPair, "%x", &value);
      // Ensure it's a valid UTF-16 character (e.g., not invalid high/low surrogate pairs)
      if (value > 0xD800 && value < 0xE000) {
        isUtf16 = false;
        break;
      }
    }
  } else {
    isUtf16 = false;  // Not a valid UTF-16 if not even length
  }

  // Determine the type
  if (isAscii) {
    return "ASCII";
  } else if (isUtf16) {
    return "UTF";
  } else {
    return "Unknown or invalid encoding.";
  }
}

String decodeAscii(const char* hex) {
  // Calculate the length of the hex string
  size_t len = strlen(hex);

  // Check if length is even
  if (len % 2 != 0) {
    return "Invalid HEX string for ASCII";
  }

  // Prepare a String object for the decoded ASCII string
  String asciiStr = "";

  // Convert hex to ASCII
  for (size_t i = 0; i < len; i += 2) {
    // Extract two characters
    char hexPair[3] = { hex[i], hex[i + 1], '\0' };

    // Convert to integer
    int value;
    sscanf(hexPair, "%x", &value);

    // Append the character to the String
    asciiStr += (char)value;
  }

  // Return the resulting ASCII string
  return asciiStr;
}