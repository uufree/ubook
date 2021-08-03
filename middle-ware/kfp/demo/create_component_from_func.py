create_step_merge_csv = kfp.components.create_component_from_func(
    func=merge_csv,
    output_component_file='component.yaml', # This is optional. It saves the component spec for future use.
    base_image='python:3.7',
    packages_to_install=['pandas==1.1.4'])