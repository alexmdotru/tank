COMPILER = gcc
CFLAGS   = -c -Wall -I include
FWORKS   = -F /Library/Frameworks
LDFLAGS  = -framework SDL2 \
					 -framework SDL2_net \
					 -framework SDL2_ttf \
					 -framework SDL2_image
CLIENT   = tank
SERVER   = tanks
OBJDIR   = build/obj
EXEDIR   = build/exe

framework.o: common/framework.c
	$(COMPILER) $(CFLAGS) $(FWORKS) $< -o $(OBJDIR)/$@

client.o: client/client.c
	$(COMPILER) $(CFLAGS) $(FWORKS) $< -o $(OBJDIR)/$@

server.o: server/server.c
	$(COMPILER) $(CFLAGS) $(FWORKS) $< -o $(OBJDIR)/$@

graphics.o: client/graphics.c
	$(COMPILER) $(CFLAGS) $(FWORKS) $< -o $(OBJDIR)/$@

network.o: common/network.c
	$(COMPILER) $(CFLAGS) $(FWORKS) $< -o $(OBJDIR)/$@

client: framework.o client.o graphics.o network.o
	$(CC) $(FWORKS) $(LDFLAGS) \
	$(OBJDIR)/framework.o \
	$(OBJDIR)/client.o \
	$(OBJDIR)/graphics.o \
	$(OBJDIR)/network.o \
	-o $(EXEDIR)/$(CLIENT)

server: server.o framework.o network.o
	$(CC) $(FWORKS) $(LDFLAGS) \
	$(OBJDIR)/framework.o \
	$(OBJDIR)/network.o \
	$(OBJDIR)/server.o \
	-o $(EXEDIR)/$(SERVER)
