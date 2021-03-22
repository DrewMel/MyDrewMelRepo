
#include "stdsoap2.h"
/* This defines the global XML namespaces[] table to #include and compile */
SOAP_NMAC struct Namespace namespaces[] = {
        { "SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL },
        { "SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL },
        { "xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL },
        { "xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL },
        { "ns3", "urn:JusWTBServiceIntf", NULL, NULL },
        { "ns4", "urn:JusWTBServiceIntf-IJusWTBService", NULL, NULL },
        { NULL, NULL, NULL, NULL}
    };
