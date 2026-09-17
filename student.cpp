#include "utils/Student.h"

#include <cstring>
#include <glm/gtc/matrix_transform.hpp>

const std::vector<Vertex> TRIANGLE = {
    { { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
    { { -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
    { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
};

VkVertexInputBindingDescription Vertex::bindingDescription() {
  VkVertexInputBindingDescription desc{};
  desc.binding = 0;
  desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  desc.stride = 20;
  return desc;
}

std::vector<VkVertexInputAttributeDescription> Vertex::attributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attrs(2);

  attrs[0].binding = 0;
  attrs[0].location = 0;
  attrs[0].format = VK_FORMAT_R32G32_SFLOAT;
  attrs[0].offset = 0;

  attrs[1].binding = 0;
  attrs[1].location = 1;
  attrs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attrs[1].offset = 8;

  return attrs;
}

struct Params {
  glm::mat4 mvp;
  std::uint32_t costLoops;
  std::uint32_t stripeWidth;
  std::uint32_t _pad[2];
};

static_assert(sizeof(Params) == 80,
              "Params must match the std140 table in the handout");

std::vector<std::uint8_t> uniformBlock(std::uint32_t costLoops,
                                       std::uint32_t stripeWidth) {
  Params params{};
  params.mvp = glm::mat4(1.0f);
  params.costLoops = costLoops;
  params.stripeWidth = stripeWidth;

  std::vector<std::uint8_t> bytes(sizeof(Params));
  std::memcpy(bytes.data(), &params, sizeof(Params));
  return bytes;
}

PipelineState pipelineState(Variant v) {
  PipelineState s{};
  s.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  s.cullMode = VK_CULL_MODE_BACK_BIT;
  s.depthCompare = VK_COMPARE_OP_GREATER;
  s.depthWrite = true;

  switch (v) {
    case Variant::EarlyZFrontToBack:
      s.fragShader = "earlyz_a.frag";
      s.drawOrder = DrawOrder::FrontToBack;
      break;
    case Variant::EarlyZBackToFront:
      s.fragShader = "earlyz_a.frag";
      s.drawOrder = DrawOrder::BackToFront;
      break;
    case Variant::WriteDepthFrontToBack:
      s.fragShader = "earlyz_b.frag";
      s.drawOrder = DrawOrder::FrontToBack;
      break;
    case Variant::WriteDepthBackToFront:
      s.fragShader = "earlyz_b.frag";
      s.drawOrder = DrawOrder::BackToFront;
      break;
  }
  return s;
}

const std::vector<Vertex> QUAD = {
    { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
    { { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
    { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
    { { -0.5f, 0.5f }, { 1.0f, 1.0f, 0.0f } },
};

const std::vector<std::uint16_t> QUAD_INDICES = {
    0, 3, 2,
    0, 2, 1,
};

std::vector<glm::mat4> instanceBuffer() {
  std::vector<glm::mat4> transforms;
  transforms.reserve(INSTANCES);

  constexpr std::uint32_t columns = 71;
  constexpr float step = 2.0f / static_cast<float>(columns);

  for (std::uint32_t i = 0; i < INSTANCES; i++) {
    std::uint32_t cx = i % columns;
    std::uint32_t cy = i / columns;
    float x = -1.0f + step * 0.5f + step * static_cast<float>(cx);
    float y = -1.0f + step * 0.5f + step * static_cast<float>(cy);

    glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
    m = glm::scale(m, glm::vec3(step * 0.5f));
    transforms.push_back(m);
  }
  return transforms;
}

void recordDraw(VkCommandBuffer cmd, std::uint32_t indexCount) {
  vkCmdDrawIndexed(cmd, indexCount, INSTANCES, 0, 0, 0);
}
