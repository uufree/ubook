import kfp
import kfp.dsl as dsl
from kfp.v2.dsl import component
from kfp import compiler
from kubernetes import client as k8s_client

@component(
    base_image="registry.sensetime.com/industry/python:alpine3.6",
    packages_to_install=[],
    output_component_file="random_op.yaml"
)
def random_op(a: int, b: int) -> int:
    import random
    return random.randint(a, b)

@component(
    base_image="registry.sensetime.com/industry/python:alpine3.6",
    packages_to_install=[],
    output_component_file="sum_op.yaml"
)
def sum_op(r1: int, r2: int) -> int:
    return r1+r2

@component(
    base_image="registry.sensetime.com/industry/python:alpine3.6",
    packages_to_install=[],
    output_component_file="mul_op.yaml"
)
def mul2_op(a: int) -> int:
    return a * 2

@component(
    base_image="registry.sensetime.com/industry/python:alpine3.6",
    packages_to_install=[],
    output_component_file="div_op.yaml"
)
def div2_op(a: int) -> int:
    return a / 2

@dsl.pipeline(
    name="random_calculate",
    description="random calculate integer"
)
def mytest() -> int:
    r1 = random_op(1, 100)
    r2 = random_op(1, 100)
    sum = sum_op(r1.outputs["Output"], r2.outputs["Output"])
    mul = mul2_op(sum.outputs["Output"])
    div = div2_op(sum.outputs["Output"])
    return sum_op(mul.outputs["Output"], div.outputs["Output"])

if __name__ == "__main__":
    pipeline_conf = kfp.dsl.PipelineConf()
    pipeline_conf.set_image_pull_secrets([k8s_client.V1ObjectReference(name="regcred")])
    compiler.Compiler(mode=kfp.dsl.PipelineExecutionMode.V2_COMPATIBLE).compile(pipeline_func=mytest,
                                                                                package_path="mytest.yaml",
                                                                                pipeline_conf=pipeline_conf)
