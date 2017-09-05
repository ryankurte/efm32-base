typedef uint8_t BackchannelState;

enum {
  NO_CONNECTION = 0,
  CONNECTION_EXISTS = 1,
  NEW_CONNECTION = 2,
  CONNECTION_ERROR = 3,
};

extern const bool backchannelSupported;
extern bool backchannelEnable;
extern int backchannelSerialPortOffset;

EmberStatus backchannelStartServer(uint8_t port);
EmberStatus backchannelStopServer(uint8_t port);
EmberStatus backchannelReceive(uint8_t port, char* data);
EmberStatus backchannelSend(uint8_t port, uint8_t * data, uint8_t length);

EmberStatus backchannelClientConnectionCleanup(uint8_t port);

BackchannelState backchannelCheckConnection(uint8_t port,
                                            bool waitForConnection);

EmberStatus backchannelMapStandardInputOutputToRemoteConnection(int port);
EmberStatus backchannelCloseConnection(uint8_t port);
EmberStatus backchannelServerPrintf(const char* formatString, ...);
EmberStatus backchannelClientPrintf(uint8_t port, const char* formatString, ...);
EmberStatus backchannelClientVprintf(uint8_t port,
                                     const char* formatString,
                                     va_list ap);
