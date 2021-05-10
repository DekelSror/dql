#include <sys/inotify.h>
#include <unistd.h>
#include <stdio.h>


#define event_size (sizeof(struct inotify_event))
#define buffer_size (1024 * (event_size + 16))


static char buffer[buffer_size] = {0};

void HandleINotifyEvent(int watcher, int watched)
{
    ssize_t i = 0;

    ssize_t length = read(watcher, buffer, buffer_size);

    printf("watcher %d, watched %d, length %ld \n", watcher, watched, length);

    while (i < length) {
        struct inotify_event *event =
            (struct inotify_event *) &buffer[i];
        if (event->len) {
            if (event->mask & IN_CREATE) {
                printf("The file %s was created.\n", event->name);
            } else if (event->mask & IN_DELETE) {
                printf("The file %s was deleted.\n", event->name);
            } else if (event->mask & IN_MODIFY) {
                printf("The file %s was modified.\n", event->name);
            }
        }
        i += event_size + event->len;
    }

}

int main(int argc, char const *argv[])
{
    int in_fd = inotify_init();


    int wd = inotify_add_watch(in_fd, ".", IN_MODIFY | IN_CREATE | IN_DELETE);

    for (size_t i = 0; i < 5; i++)
    {
        HandleINotifyEvent(in_fd, wd);
    }

    (void) inotify_rm_watch(in_fd, wd);
    (void) close(in_fd);
    
    return 0;
}
