MMDSP_SRCS_PATH = src
MMDSP_SRCS      = $(wildcard $(MMDSP_SRCS_PATH)/*.c)
MMDSP_INC_PATH  = inc
MMDSP_OBJS      = $(MMDSP_SRCS:$(MMDSP_SRCS_PATH)/%.c=objs/%.o)
# this should also include the dependencies of external libraries
MMDSP_DEP       = $(wildcard $(MMDSP_INC_PATH)/*.h)
MMDSP_CFLAGS	= -Ofast

# requires mm_primitives library
# assumes it is contained in the parent directory
MMPRIMITIVES_PATH = ../mm_primitives
MMPRIMITIVES_INC_PATH = $(MMPRIMITIVES_PATH)/inc
INC		= $(MMDSP_INC_PATH) $(MMPRIMITIVES_INC_PATH)
CFLAGS	= $(foreach inc,$(INC),-I$(inc))

CFLAGS += -ggdb3 -gdwarf-4 -Wall -ffunction-sections -fdata-sections
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -dD
CFLAGS += -Wno-unused-function

ARFLAGS   = rcs

OBJSDIR = objs

LIBDIR  = lib

CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar

BIN = $(LIBDIR)/libmm_dsp.a


all: $(LIBDIR) $(OBJSDIR) $(BIN) 

$(LIBDIR):
	if [ ! -d "$(LIBDIR)" ]; then mkdir $(LIBDIR); fi

$(OBJSDIR):
	if [ ! -d "$(OBJSDIR)" ]; then mkdir $(OBJSDIR); fi

# compile mmdsp 
$(MMDSP_OBJS): $(OBJSDIR)/%.o: $(MMDSP_SRCS_PATH)/%.c $(MMDSP_DEP)
	$(CC) -c $(CFLAGS) $< -o $@ $(MMDSP_CFLAGS)

$(BIN): $(MMDSP_OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -r $(LIBDIR) $(OBJSDIR)
