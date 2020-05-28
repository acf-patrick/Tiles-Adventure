import pygame as pg

class Timer:
    def __init__(self):
        self.restart()
        self.stopped = False
    def stop(self):
        self.stopped = True
    def get_elapsed(self):
        if self.stopped:
            return 0
        return pg.time.get_ticks() - self.start_time
    def restart(self):
        self.stopped = False
        self.start_time = pg.time.get_ticks()
    
pg.init()
screen = pg.display.set_mode((100, 100))

class Ghost(pg.sprite.Sprite):
    def __init__(self):
        self.rect = pg.rect.Rect(0, 0, 44, 30)
        self.image = self.appear = pg.image.load("Appear (44x30).png")
        self.disappear = pg.image.load("Desappear (44x30).png")
        self.idle = pg.image.load("Idle (44x30).png")
        self.state = 0
        self.appearing = True
        self.disappearing = False
        self.timer = Timer()
        self.state_timer = Timer()
        self.state_timer.stop()

    def draw(self, screen):
        screen.blit(self.image, (0, 0), ((self.state*self.rect.w, 0), (self.rect.w, self.rect.h)))

    def update(self):
        if self.state_timer.get_elapsed() >= 3000:
            self.state_timer.stop()
            self.disappearing = True
            self.state = 0
            self.timer.restart()
        if self.disappearing:
            self.image = self.disappear
        else:
            self.image = self.appear if self.appearing else self.idle
        nbr = self.image.get_width()/self.rect.w
        if self.timer.get_elapsed() >= 80:
            self.timer.restart()
            self.state += 1
            if self.appearing and self.state >= nbr:
                self.appearing = False
                self.state_timer.restart()
        if not self.disappearing:
            self.state %= nbr

name = "Appear (44x30).png"
size = name[name.find('(')+1:-5]
x_pos = size.find('x')
w = int(size[:x_pos])
h = int(size[x_pos+1:])
spritesheet = pg.transform.flip(pg.image.load(name), False, False)
nbr = spritesheet.get_width() / w
cur = 0

done = False
timer = Timer()
ghost = Ghost()
while not done:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            done = True
    ghost.update()
    screen.fill(0)
    ghost.draw(screen)
    pg.display.flip()

pg.quit()
