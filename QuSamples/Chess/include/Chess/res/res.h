#pragma once

#include <QuEngine/QuBase64.h>

#ifdef _DEBUG
#define b64_bin(x) b64_bin_runtime(x)
#else
#define b64_bin(x) b64_bin_comptime(x)
#endif

auto IMG_BOARD = b64_bin(
  R"(iVBORw0KGgoAAAANSUhEUgAAAgAAAAIABAMAAAAGVsnJAAAAElBMVEX/zp7Ri0f8yZjUkE35xZLXlFNZsZNlAAAC1klEQVR42uzcsQmAMBAF0OAGwQVEXECCC7iC++9iYepYBXLk/f4g90h7P/1kOXMz6xZ7PgMAEHsBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAVIC9neMpzVx37PmSytgP7D2/pzz2F+09nwAAiL0AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA1zuenL1CYvkIDQOwFAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA8MX5/PQFCtNXaACIvQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAC87d0wEAACDQMy/6w5VwX1wQHYAAAAAAAAAAAAAAAAAAAAAAAAA8DGfzx8o5C80AGwXAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwMd8Pn+gkL/QALBdAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB/z+fyBQv5CA8B2AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHzM5/MHCvkLDQDbBQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAK4dOiABAAAAEPT/ddAOnaAAAQIECBAgQIAAAQIECBAgQIAAAQIECBAgQIAAAQIECBAgQIAAAQIELEEfuS1Bf8MXAAAAAElFTkSuQmCC)");

auto IMG_PIECES = b64_bin(
  R"(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)");

auto IMG_SQUARE = b64_bin(
  R"(iVBORw0KGgoAAAANSUhEUgAAAEAAAABAAQMAAACQp+OdAAAAA1BMVEX/gYGhxROLAAAAAXRSTlOArV5bRgAAAA5JREFUKM9jGAWjgAIAAAJAAAFSSwuTAAAAAElFTkSuQmCC)");

auto IMG_CIRCLE = b64_bin(
  R"(iVBORw0KGgoAAAANSUhEUgAAAEAAAABABAMAAABYR2ztAAAAHlBMVEUAAABiYmBjY2FjY2JmZmViYmBlZWRmZmVjY2FkZGJiqUkfAAAACnRSTlMAf2szC3cpGVpR+3B3pQAAAKZJREFUSMdjGAWjADdwTzYrwSPNYqkaKBo02QGnvJEgGCjjUlEhCAXt2OXZFGEKhBKwKkgVhIMwbPLsiggFQgVYFHgIIoEWLAoMkRUIY/GjIrICIUyfMguiAAMMBZyoCiZgKHBCVaCCoaAUVUE49mDCF1SGqAqECSsgbAVhRxL2JuGAIhzUhCOLcHQTTjCEkxzhREs42WNmHDKzHiLzEsr+o2AU4AQAa9csO8RTntAAAAAASUVORK5CYII=)");

auto SFX_TACOBELL = b64_bin(
  R"(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ommqPM8TRVEUTVNViaLoiaIomqaqkmVRNE3TVFXXZcuiaJqmqaquC9MURVV1XdmFaYqiabquLEO2VVNVXVe2YdumqaquK8vAdV1Xlm0duK7ryrKtCwAAT3AAACqwYXWEk6KxwEJDVgIAGQAABCEIKaUQUkohpJRCSCmFkAAAgAEHAIAAE8pAoSErAYBUAAAAQgghhBBCCCGEEEIIIYQQQgghhBBCCCGEEEIIIYQQQgghhBBCCCGEEEIIIYQQQgghhNA555xzzjnnnHPOOeecc84555xzzjnnnHPOOeecc84555xzzjnnnHPOOeecc84555xzzjnnnHNOACB2hQPAToQNqyOcFI0FFhqyEgAIBwAAjDHGOYu11lprpZRSEmqttdZaM4WUktBijDHGGDMGIaUWY4wxxpgx56jFGGOMMcbWSoktxhhjjDG2VkqMMcYYY4wxxthiizHGGGOMMcYWY4wxxhhjjDHGGGOMMcYYY4wxxhZjjDHGGGOMMcYYY4wxxhhjjDHGGGOMMcYYY4wtxhhjjDHGGGOMMcYYY4wFAJg8OABAJdg4w0rSWeFocKEhKwGA3AAAwBilGHPMOQghlFJCKam1zjkHIYRSSikplZZSTBlzzjkIoZRSQikptZQ65xyEUkpJKaWUUkutdQ5CCKGUUkpJKaWUWgohhFJKSSWllFJqrbUUQgillJJSSimllFprMZQSUimlpJRSKqml1lJLJZSSSkoppZRSai211kopqaSUUkoppZRabC2FUlIqKaWWUkqptRhbLKWVlFJKKaXUWoqttRZbSiml1FJLKaXWYkutpZRSaiml1FJqqcXYWmsppZRaSi21lFKKrbUWU0qtpZRaaq2lFltKraWWUmottZRSa63FFltrLaWWUkoptdZiSzG2llpJKaWWWkutxdZia6211FpLLaXUYosxxthibC2mlFJqKbVUAADQgQMAQIARlRZipxlXHoEjChkmoEJDVgIAZAAABDJNMicpNcIkpxiU0pxzSimllIbIkgxSDKojkzEnKWeINIYUpJ4p8phSDGIISYVOMYetJh9L6CDWoIwRLqUYAAAAQQCAgJAAAAMEBTMAwOAAYeRAoCOAwKENADAQITOBQSE0OMgEgAeICKkAIDFBUbrQBSFEkC6CLB64cOLGEzec0KENBAAAAAAABAA+AAASCiAgIpq5CosLjAyN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)");

auto SFX_VINEBOOM = b64_bin(
  R"(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)");

#define raw_arr(x) x.data(), x.size()