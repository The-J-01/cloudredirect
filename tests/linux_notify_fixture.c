#include <string.h>

struct NotifyNotification {
    int critical;
    int urgency;
    int timeout;
};

int notify_init(const char* name) {
    return strcmp(name, "CloudRedirect") == 0;
}

struct NotifyNotification* notify_notification_new(const char* summary, const char* body, const char* icon) {
    static struct NotifyNotification notification;
    if (strcmp(summary, "CloudRedirect") != 0 || icon)
        return 0;
    if (strcmp(body, "critical") == 0)
        notification.critical = 1;
    else if (strcmp(body, "normal") != 0)
        return 0;
    return &notification;
}

void notify_notification_set_urgency(struct NotifyNotification* notification, int urgency) {
    notification->urgency = urgency;
}

void notify_notification_set_timeout(struct NotifyNotification* notification, int timeout) {
    notification->timeout = timeout;
}

struct GError;
int notify_notification_show(struct NotifyNotification* notification, struct GError** error) {
    return !error && (notification->critical
        ? notification->urgency == 2 && notification->timeout == 0
        : notification->urgency == 1 && notification->timeout == 5000);
}
