import kfp
import kfp.dsl as dsl
from kfp.v2.dsl import component
from kfp.v2.dsl import (
    Input,
    Output,
    Artifact,
    Dataset,
)
from kfp import compiler


@component(
    packages_to_install=['pandas==1.1.4'],
    output_component_file='component.yaml'
)
def merge_csv(tar_data: Input[Artifact], output_csv: Output[Dataset]):
    import glob
    import pandas as pd
    import tarfile

    tarfile.open(name=tar_data.path, mode="r|gz").extractall('data')
    df = pd.concat(
        [pd.read_csv(csv_file, header=None)
         for csv_file in glob.glob('data/*.csv')])
    df.to_csv(output_csv.path, index=False, header=False)


web_downloader_op = kfp.components.load_component_from_url(
    'https://raw.githubusercontent.com/kubeflow/pipelines/master/components/web/Download/component-sdk-v2.yaml')


# Define a pipeline and create a task from a component:
@dsl.pipeline(
    name='my-pipeline',
    # You can optionally specify your own pipeline_root
    # pipeline_root='gs://my-pipeline-root/example-pipeline',
)
def my_pipeline(url: str):
    web_downloader_task = web_downloader_op(url=url)
    merge_csv_task = merge_csv(tar_data=web_downloader_task.outputs['data'])
    # The outputs of the merge_csv_task can be referenced using the
    # merge_csv_task.outputs dictionary: merge_csv_task.outputs['output_csv']

if __name__ == "__main__":
    kfp.compiler.Compiler(mode=kfp.dsl.PipelineExecutionMode.V2_COMPATIBLE).compile(
        pipeline_func=my_pipeline,
        package_path='pipeline.yaml')