#include <io.h>
#include <time.h>
#include <string.h>
#include <command.h>
#include <panic.h>
#include <assert.h>

#include <errno.h>

/* panic leds using the configuration of the tdc board */
struct panic_ledinfo panic_ledinfo = {
	.gpio_clock = GPIO_NR(0, 7),
	.gpio_data = GPIO_NR(1, 28),
};

static int command_go(char **reply, int argc, char **argv)
{
	int addr;
	char c;
	void (*f)(void);

	if (sscanf(argv[1], "%x%c", &addr, &c) != 1)
		return -EINVAL;
	f = (void *)addr;
	printf("going to %p\n", f);
	f();
	return 0;
}

static int command_assert(char **reply, int argc, char **argv)
{
	int value;
	char c;

	if (sscanf(argv[1], "%i%c", &value, &c) != 1)
		return -EINVAL;
	assert_warn(!value, value, "received value was %i\n", value);
	assert(value, 48, "received value was %i\n", value);
	return 0;
}

static int command_panic(char **reply, int argc, char **argv)
{
	int value;
	char c;

	if (sscanf(argv[1], "%i%c", &value, &c) != 1)
		return -EINVAL;
	panic(value, "Panic %i requested by command\n", value);
	return 0;
}

static int command_mdelay(char **reply, int argc, char **argv)
{
	unsigned long j;
	int ms;
	char c;

	if (sscanf(argv[1], "%i%c", &ms, &c) != 1)
		return -EINVAL;
	if (ms < 0)
		return -EINVAL;
	j = jiffies + ms_to_jiffies(ms);
	while (time_before(jiffies, j))
		;
	return 0;
}

static int command_udelay(char **reply, int argc, char **argv)
{
	int us;
	char c;

	if (sscanf(argv[1], "%i%c", &us, &c) != 1)
		return -EINVAL;
	if (us < 0)
		return -EINVAL;
	udelay(us);
	return 0;
}
static init command_buzzer(char**reply, int argc, char**argv) 
{
        int frequence;
        int time;
        char c;
        
        if (sscanf(argv[1], "%i%c", &frequence, &time, &c) !=2)
            return -EINVAL;  
        if (frequence<=0 || time <=0)
            return -EINVAL;
        
        printf("playing a tune with the buzzer at %dHz for %dms\n", frequence, time);
        buzzer_play(frequence, time);
        
        return 0;
}
static struct command shell_commands[] = {
	COMMAND_R,
	COMMAND_W,
	COMMAND_GPIO,
	COMMAND_COMMIT,
	COMMAND_HELP,
	{"go",    command_go,      2, 2},
	{"assert", command_assert, 2, 2},
	{"panic",  command_panic,  2, 2},
	{"mdelay", command_mdelay, 2, 2},
	{"udelay", command_udelay, 2, 2},
	{"buzzer", command_udelay, 2, 2},
	{}
};

void main(void)
{
	static char str[80];
	printf("%s: built on %s-%s\n", __FILE__, __DATE__, __TIME__);
	while (1) {

		if (polls(str, sizeof(str))) {
			command_parse(str, shell_commands);
			puts(command_reply);
			str[0] = '\0';
		}
	}
}
