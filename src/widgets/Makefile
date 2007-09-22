SRCDIR := ../..
SUBDIRS :=

LIBNAME := widgets.a

SRCS := \
	widget.c \
	color.c \
	text.c \
	image.c \
	button.c \
	smart_clock.c \
	smart_entry_box.c \
	smart_menu.c \
	smart_textblock.c \
	smart_box_yesno.c \
	smart_animator.c \

include $(SRCDIR)/Makefile.common

CFLAGS += -I..

all:: depend $(LIBNAME)

clean::
	$(RM) $(LIBNAME)
