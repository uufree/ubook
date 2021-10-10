# FOCUS

[TOC]

中间件列表：

- redis 20d
- kafka 10d
- nginx 20d
- mysql 20d

## Schedule

- **LeetCode** >> 06:30～08:30 
- **基础组件** >> 09:00～10:00，12:30～13:30 
- **流媒体** >> 19:00～21:00

## Partation 00

- 时间：**2021.09.01～2021.09.30**
  - 基础开发组件
    - **Redis**：
      - 极客时间：《Redis核心技术和实战》
      - 阅读官方文档，总结并熟悉Redis的方方面面
      - 阅读并总结：《Redis实战》，《Redis深度历险》，《Redis设计与实现》
      - 阅读Redis源码，并进行注释
      - 熟练使用Golang操作Redis
      - 熟悉Redis运维与部署
      - 拓展：理解Redis分布式设计方案
      - 面经
    - **Kafka**：
      - 极客时间：《Kafka核心技术与实战》
      - 阅读官方文档，总结并熟悉Kafka的方方面面
      - 阅读并总结：《Kafka权威指南》
      - 熟练使用Golang操作Kafka
      - 熟悉Kafka运维与部署
      - 拓展：理解Kafka分布式设计方案
      - 面经
  - 流媒体开发：
    - 阅读并注释ffmpeg、ffplay
    - 总结主要的ffmpeg-interface，并给出主干函数demo
    - 使用c++开发完成所有的ffmpeg-manual-samples
    - 使用c++开发一个视频播放器
    - 阅读并总结雷神的音视频blog
    - 总结H.264和H.265，并进行对比
    - ffmpeg面经





## 需求

需要在满足模型训练接口（https://www.figma.com/file/wpMe0Zw22LwMdO1PW5k7Ta/Spock%E5%B9%B3%E5%8F%B0-V0.2%E7%89%88%E6%9C%AC%E3%80%90%E5%BD%92%E6%A1%A3%E3%80%91?node-id=761%3A2451）的前提下，开发通用的pipeline、expirment、run service。

### pipeline

在模型训练v0.2中，对pipeline的需求如下：

- Create：后台或者初始化脚本使用，不面向前端页面。这个接口和需求无关，需求再怎么改，这个接口也不会变。

  ```protobuf
  message Parameter {
    string name = 1;
    string value = 2;
  }
  
  message Pipeline {
    string id = 1;
    string name = 2;
    string description = 3;
    repeated Parameter parameters = 4;
    google.protobuf.Timestamp created_at = 5;
  }
  
  message CreatePipelineRequest {
    string name = 1;
    string description = 2;
    string artifact_download_id = 3;
  }
  
  message CreatePipelineResponse {
    string id = 1;
  }
  ```

- Delete：后台或者初始化脚本使用，不面向前端页面。这个接口和需求无关，需求再怎么改，这个接口也不会变。

  ```protobuf
  message DeletePipelineRequest {
    string id = 1;
  }
  
  message DeletePipelineResponse {}
  ```

- Get：前端页面使用。

  ```protobuf
  message PipelineDetailRequest {
    string id = 1;
  }
  
  message PipelineDetailResponse {
    Pipeline pipeline = 1;
  }
  ```

- List：前端页面使用。

  ```protobuf
  message ListPipelineRequest {
    string page_token = 1;
    int32 page_size = 2;
  }
  
  message ListPipelineResponse {
    string next_page_token = 1;
    int32 total = 2;
    repeated Pipeline pipelines = 3;
  }
  ```

对于上述需求，请看kfp pipeline.proto：

```protobuf
  // Finds a specific pipeline by ID.
  rpc GetPipeline(GetPipelineRequest) returns (Pipeline) {
    option (google.api.http) = {
      get: "/apis/v1beta1/pipelines/{id}"
    };
  }

  // Finds all pipelines.
  rpc ListPipelines(ListPipelinesRequest) returns (ListPipelinesResponse) {
    option (google.api.http) = {
      get: "/apis/v1beta1/pipelines"
    };
  }
  
  message GetPipelineRequest {
  	// The ID of the pipeline to be retrieved.
  	string id = 1;
	}
	
	message ListPipelinesRequest {
    // A page token to request the next page of results. The token is acquried
    // from the nextPageToken field of the response from the previous
    // ListPipelines call.
    string page_token = 1;

    // The number of pipelines to be listed per page. If there are more pipelines
    // than this number, the response message will contain a valid value in the
    // nextPageToken field.
    int32 page_size = 2;

    // Can be format of "field_name", "field_name asc" or "field_name desc"
    // Ascending by default.
    string sort_by = 3;

    // A url-encoded, JSON-serialized Filter protocol buffer (see
    // [filter.proto](https://github.com/kubeflow/pipelines/
    // blob/master/backend/api/filter.proto)).
    string filter = 4;

    // What resource reference to filter on.
    // For Pipeline, the only valid resource type is Namespace. An sample query string could be
    // resource_reference_key.type=NAMESPACE&resource_reference_key.id=ns1
    ResourceKey resource_reference_key = 5;
	}

	message ListPipelinesResponse {
    repeated Pipeline pipelines = 1;

    // The total number of pipelines for the given query.
    int32 total_size = 3;

    // The token to list the next page of pipelines.
    string next_page_token = 2;
	}
```

从接口上看，kfp完全满足我们的需求，故不需要额外的业务层表。

### experiment

需求：Create、Delete、Get、List

- Create：前端使用，对应：创建模型
  - input：name，description
  - output：experiment id
- Delete：前端使用，对应：删除模型
  - input：experiment id
  - output：none
- Get：前端使用：对应模型细节
  - input：experiment id
  - output：
    - experiment name
    - experiment description
    - kfp run ids
- List：前端使用，对应模型列表
  - input：pager，name
  - output：experiments

综上，对于experiment接口，仅需调用kfp接口即可。















