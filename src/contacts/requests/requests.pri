INCLUDEPATH += requests

PUBLIC_HEADERS += \
    requests/qcontactfetchrequest.h \
    requests/qcontactfetchbyidrequest.h \
    requests/qcontactidfetchrequest.h \
    requests/qcontactsaverequest.h \
    requests/qcontactremoverequest.h \
    requests/qcontactrelationshipfetchrequest.h \
    requests/qcontactrelationshipremoverequest.h \
    requests/qcontactrelationshipsaverequest.h \
    requests/qcontactrequests.h

PRIVATE_HEADERS += \
    requests/qcontactrequests_p.h

SOURCES += \
    requests/qcontactfetchrequest.cpp \
    requests/qcontactfetchbyidrequest.cpp \
    requests/qcontactidfetchrequest.cpp \
    requests/qcontactsaverequest.cpp \
    requests/qcontactremoverequest.cpp \
    requests/qcontactrelationshipfetchrequest.cpp \
    requests/qcontactrelationshipremoverequest.cpp \
    requests/qcontactrelationshipsaverequest.cpp
