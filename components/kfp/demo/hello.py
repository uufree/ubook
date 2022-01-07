import kfp
from kfp.v2 import dsl
from kfp import compiler
from kfp import components

# hello component
def hello_world(text: str):
    print(text)
    return text

hello_op = components.create_component_from_func(
    func=hello_world,
    base_image="registry.sensetime.com/industry/python:alpine3.6",
    packages_to_install=[],
    output_component_file="div_op.yaml",
)

@dsl.pipeline(name='hello-world', description='A simple intro pipeline')
def pipeline_hello_world(text: str = 'hi there'):
    '''Pipeline that passes small pipeline parameter string to consumer op'''

    consume_task = hello_world(
        text
    )  # Passing pipeline parameter as argument to consumer op


if __name__ == "__main__":
    # execute only if run as a script
    compiler.Compiler(mode=kfp.dsl.PipelineExecutionMode.V2_COMPATIBLE).compile(
        pipeline_func=pipeline_hello_world,
        package_path='hello_world_pipeline.yaml'
    )
