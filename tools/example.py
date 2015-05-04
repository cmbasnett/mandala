from mandala import *

class ConsoleState(GuiState):
	def __init__(self):
		GuiState.__init__(self)

		bitmap_font = resources.get_bitmap_font('unifont_16.fnt')

		root_node = GuiNode()
		root_node.dock_mode = GuiDockMode.TOP
		root_node.set_size(Vec2F(0.5, 0.5), GuiSizeMode.RELATIVE)

		output_root_node = GuiNode()
		output_root_node.dock_mode = GuiDockMode.FILL
		output_root_node.should_clip = True

		output_scroll = GuiScroll()
		output_scroll.dock_mode = GuiDockMode.FILL
		output_scroll.margin = Padding(8, 8, 8, 8)

		root_background_image = GuiImage()
		root_background_image.color = Vec4F(0, 0, 0, 0.75)
		root_background_image.dock_mode = GuiDockMode.FILL
		root_background_image.sprite = Sprite(Hash('white.tpsb'), Hash('white.png'))

		output_label = GuiLabel()
		output_label.bitmap_font = bitmap_font
		output_label.justification = GuiLabel.Justification.LEFT
		output_label.should_use_ellipses = False
		output_label.should_use_color_codes = True
		output_label.vertical_alignment = GuiLabel.VerticalAlignment.BOTTOM
		output_label.is_autosized_to_text = True
		output_label.dock_mode = GuiDockMode.BOTTOM

		input_root_node = GuiNode()
		input_root_node.dock_mode = GuiDockMode.BOTTOM
		input_root_node.set_size(Vec2F(0, bitmap_font.line_height + 16))

		input_background_image = GuiImage()
		input_background_image.color = Vec4F(0, 0, 0, 1)
		input_background_image.dock_mode = GuiDockMode.FILL
		input_background_image.sprite = Sprite(Hash('white.tpsb', Hash('white.png')))

		input_label = GuiLabel()
		input_label.bitmap_font = bitmap_font
		input_label.dock_mode = GuiDockMode.FILL
		input_label.should_use_color_codes = False
		input_label.should_use_ellipses = False
		input_label.is_read_only = True
		input_label.margin = Padding(0, 8, 0, 8)
		input_label.vertical_alignment = GuiLabel.VerticalAlignment.MIDDLE
		input_label.color = Vec4F(0, 1, 0, 1)

		output_scroll.adopt(output_label)
		output_root_node.adopt(output_scroll)

		input_root_node.adopt(input_background_image)
		input_root_node.adopt(input_label)

		root_node.adopt(root_background_image)
		root_node.adopt(input_root_node)
		root_node.adopt(output_root_node)

		self.layout.adopt(root_node)

states.push(s, STATE_FLAG_ALL)
states.pop(s)

print states.index_of(s)