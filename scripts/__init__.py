import GameEngine

renderableInst = None

class MyRenderable(GameEngine.Renderable):
	def __init__(self):
		self.count = 0

	def Update(self):
		return True

	def Render(self):
		self.count += 1
		print(self.count)


def init():
	global renderableInst
	renderableInst = MyRenderable()
	GameEngine.AddRenderable(renderableInst, GameEngine.Layers["LEVEL"])
