extends Camera

# Sensitivity for mouse movement
var mouse_sensitivity : float = 0.3
# Speed for camera movement
var move_speed : float = 50.0

# Rotation angles
var yaw : float = 0.0
var pitch : float = 0.0

func _ready():
	# Set initial mouse mode to visible
	Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)

func _unhandled_input(event):
	if event is InputEventMouseMotion and Input.get_mouse_mode() == Input.MOUSE_MODE_CAPTURED:
		yaw -= event.relative.x * mouse_sensitivity
		pitch -= event.relative.y * mouse_sensitivity
		# Clamp the pitch to prevent flipping
		pitch = clamp(pitch, -89, 89)
		rotation_degrees = Vector3(pitch, yaw, 0)

func _process(delta):
	var direction : Vector3 = Vector3()

	# WASD keys for movement (horizontal plane)
	if Input.is_action_pressed("ui_up"):
		direction -= transform.basis.z
	if Input.is_action_pressed("ui_down"):
		direction += transform.basis.z
	if Input.is_action_pressed("ui_left"):
		direction -= transform.basis.x
	if Input.is_action_pressed("ui_right"):
		direction += transform.basis.x

	# Normalize horizontal direction to ensure consistent speed in all directions
	direction.y = 0
	direction = direction.normalized()

	# Q and Z keys for vertical movement
	if Input.is_action_pressed("move_up"):
		direction.y += 1
	if Input.is_action_pressed("move_down"):
		direction.y -= 1

	# Move the camera
	translation += direction * move_speed * delta

func _input(event):
	if event is InputEventMouseButton and event.button_index == BUTTON_LEFT and event.pressed:
		Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	elif event is InputEventKey:
		if event.scancode == KEY_ESCAPE:
			Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)

# Set up input actions in project settings:
# ui_up (W key)
# ui_down (S key)
# ui_left (A key)
# ui_right (D key)
# move_up (Q key)
# move_down (Z key)
