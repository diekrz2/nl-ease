#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>

static int lock_fd = -1;
static int daemon_lock_fd = -1;

int
lock_gui(void)
{
    char path[256];
    const char *home = getenv("HOME");

    if (!home)
        home = "/tmp";

    char base_dir[256], dir[256];
    snprintf(base_dir, sizeof(base_dir), "%s/.config", home);
    mkdir(base_dir, 0755);
    snprintf(dir, sizeof(dir), "%s/.config/nl-ease", home);
    mkdir(dir, 0755);

    snprintf(path,
             sizeof(path),
             "%s/.config/nl-ease/nl-ease.lock",
             home);

    lock_fd = open(path, O_CREAT | O_RDWR, 0644);

    if (lock_fd < 0)
    {
        perror("open");
        return 0;
    }

    if (flock(lock_fd, LOCK_EX | LOCK_NB) != 0)
    {
        close(lock_fd);
        lock_fd = -1;

        fprintf(stderr, "Another nl-ease instance is already running.\n");
        return 0;
    }

    return 1;
}

void
unlock_gui(void)
{
    if (lock_fd >= 0)
    {
        flock(lock_fd, LOCK_UN);
        close(lock_fd);
        lock_fd = -1;
    }
}

int
lock_daemon(void)
{
    char path[256];
    const char *home = getenv("HOME");

    if (!home)
        home = "/tmp";

    char base_dir[256], dir[256];
    snprintf(base_dir, sizeof(base_dir), "%s/.config", home);
    mkdir(base_dir, 0755);
    snprintf(dir, sizeof(dir), "%s/.config/nl-ease", home);
    mkdir(dir, 0755);

    snprintf(path,
             sizeof(path),
             "%s/.config/nl-ease/nl-ease-daemon.lock",
             home);

    daemon_lock_fd = open(path, O_CREAT | O_RDWR, 0644);

    if (daemon_lock_fd < 0)
    {
        perror("open");
        return 0;
    }

    if (flock(daemon_lock_fd, LOCK_EX | LOCK_NB) != 0)
    {
        close(daemon_lock_fd);
        daemon_lock_fd = -1;

        fprintf(stderr, "Another nl-ease daemon is already running.\n");
        return 0;
    }

    return 1;
}

void
unlock_daemon(void)
{
    if (daemon_lock_fd >= 0)
    {
        flock(daemon_lock_fd, LOCK_UN);
        close(daemon_lock_fd);
        daemon_lock_fd = -1;
    }
}
