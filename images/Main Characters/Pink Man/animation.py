import pygame as pg

class Timer:
    def __init__(self):
        self.restart()
    def get_elapsed(self):
        return pg.time.get_ticks() - self.start_time
    def restart(self):
        self.start_time = pg.time.get_ticks()
    
pg.init()
screen = pg.display.set_mode((100, 100))

name = "Run (32x32).png"
size = name[name.find('(')+1:-5]
x_pos = size.find('x')
w = int(size[:x_pos])
h = int(size[x_pos+1:])
spritesheet = pg.transform.flip(pg.image.load(name), True, False)
nbr = spritesheet.get_width() / w
cur = 0

done = False
timer = Timer()
while not done:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            done = True
    if timer.get_elapsed() >= 75:
        cur += 1
        cur %= nbr
        timer.restart()
    screen.fill(0xff)
    screen.blit(spritesheet, (0, 0), area = ((w*cur, 0), (w, h)))
    pg.display.flip()

pg.quit()
