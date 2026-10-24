#include "Renderer.h"

void Renderer::UploadBuffer(const VertexBuffer& vertex_buffer) const
{
    glNamedBufferSubData(m_VertexBuffer, 0, vertex_buffer.Size(), vertex_buffer.m_Buffer.data());
}

void Renderer::Clear(GLbitfield flags) const
{
    glClear(flags);
}

void Renderer::Flush() const
{
    glFlush();
}

void Renderer::ParticleSystemSync(Scene* scene)
{
    EnsureParticleAllocation(scene);
    ReinstantiateParticleInstances(scene);
    UpdateGPUParticleStates(scene);
}

uint32_t Renderer::AllocateCurves(uint32_t num_curves)
{
    auto& allocatedRanges = m_AllocatedCurveRanges;
    auto& freeRanges = m_FreeCurveRanges;
    return AllocateRange(num_curves, allocatedRanges, freeRanges);
}

bool Renderer::DeallocateCurves(primitives::Range range)
{
    auto& allocatedRanges = m_AllocatedCurveRanges;
    auto& freeRanges = m_FreeCurveRanges;
    return DeallocateRange(range, allocatedRanges, freeRanges);
}

void Renderer::MergeFreeCurveRanges()
{
    auto& freeRanges = m_FreeCurveRanges;
    return MergeFreeRanges(freeRanges);
}

uint32_t Renderer::AllocateParticles(uint32_t num_particles)
{
    /*/
    uint32_t currentOffset = -1;
    for (auto rangeIterator = m_FreeRanges.begin(); rangeIterator != m_FreeRanges.end(); rangeIterator++)
    {
        if (num_particles <= rangeIterator->Size)
        {
            currentOffset = rangeIterator->Offset;
            primitives::Range allocatedRange{ currentOffset, (num_particles)};            
            m_AllocatedRanges.push_back(allocatedRange);
            rangeIterator->Offset += num_particles;
            rangeIterator->Size -= num_particles;
            if (rangeIterator->Size == 0)
            {
                m_FreeRanges.erase(rangeIterator);
            }
            return currentOffset;
        }
    }
    return currentOffset;
    /**/
    auto& allocatedRanges = m_AllocatedParticleRanges;
    auto& freeRanges = m_FreeParticleRanges;
    return AllocateRange(num_particles, allocatedRanges, freeRanges);
}

bool Renderer::DeallocateParticles(primitives::Range range)
{
    /*/
    auto rangeIter = std::find(m_AllocatedParticleRanges.begin(), m_AllocatedParticleRanges.end(), range);
    if (rangeIter == m_AllocatedParticleRanges.end())
    {
        return false;
    }
    m_AllocatedParticleRanges.erase(rangeIter);
    auto it = m_FreeParticleRanges.begin();
    while (it != m_FreeParticleRanges.end() && it->Offset < range.Offset)
    {
        ++it;
    }
    m_FreeParticleRanges.insert(it, range);
    MergeFreeRanges();
    return true;
    /**/
    auto& allocatedRanges = m_AllocatedParticleRanges;
    auto& freeRanges = m_FreeParticleRanges;
    return DeallocateRange(range, allocatedRanges, freeRanges);
}

void Renderer::MergeFreeParticleRanges()
{
    /*/
    if (m_FreeParticleRanges.size() <= 1) return;
    for (std::size_t index = 0; index < m_FreeParticleRanges.size() - 1;)
    {
        auto& current = m_FreeParticleRanges[index];
        auto& next = m_FreeParticleRanges[index + 1];
        if (current.Offset + current.Size >= next.Offset)
        {
            current.Size = glm::max<uint32_t>(current.Size, (next.Offset + next.Size) - current.Offset);
            m_FreeParticleRanges.erase(m_FreeParticleRanges.begin() + (index + 1));
        }
        else
        {
            ++index;
        }
    }
    /**/
    auto& freeRanges = m_FreeParticleRanges;
    return MergeFreeRanges(freeRanges);
}

Renderer::Renderer() :
    counter{ 0 },
    counterLimit{ 64 },
    m_Ts{ 0 },
    m_VertexBuffer{ 0 },
    m_IndexBuffer{ 0 },
    m_MaterialBuffer{ 0 },
    m_ModelMatrixInstanceBuffer{ 0 },
    m_InstanceGroups{},
    m_InstanceGroupKeys{},
    m_InstanceGroupsMap{},
    m_AllocatedParticleRanges{},
    m_FreeParticleRanges{ {0, 16384} }
{
    GLsizeiptr vertex_buffer_size = sizeof(primitives::Vertex) * MAX_CHARACTER_BUFFER_SIZE;
    GLsizeiptr index_buffer_size = sizeof(unsigned int) * MAX_CHARACTER_BUFFER_SIZE;
    GLsizeiptr material_buffer_size = sizeof(MTLMaterial) * MAX_MATERIALBATCH_BUFFER_SIZE;
    GLsizeiptr modelMatrix_buffer_size = sizeof(glm::mat4) * MAX_INSTANCEBATCH_BUFFER_SIZE;
    GLsizeiptr maxParticleBufferSize = sizeof(glm::vec4) * MAX_PARTICLE_BUFFER_SIZE;
    GLsizeiptr maxControlPointBufferSize = sizeof(glm::vec4) * MAX_CONTROL_POINT_BUFFER_SIZE;
    GLsizeiptr maxKnotBufferSize = sizeof(float) * MAX_KNOT_BUFFER_SIZE;
    std::cout << "OpenGL Render : Init\n";
    std::cout << "\nGeometry***\n";
    glCreateBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glNamedBufferData(m_VertexBuffer, vertex_buffer_size, nullptr, GL_DYNAMIC_DRAW);
    std::cout << "\tglCreate : Vertex Buffer\n";
    glCreateBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glNamedBufferData(m_IndexBuffer, index_buffer_size, nullptr, GL_DYNAMIC_DRAW);
    std::cout << "\tglCreate : Index Buffer\n";
    glCreateBuffers(1, &m_MaterialBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_MaterialBuffer);
    glNamedBufferData(m_MaterialBuffer, material_buffer_size, nullptr, GL_DYNAMIC_DRAW);
    std::cout << "\tglCreate : SSBO - Material Buffer\n";
    glCreateBuffers(1, &m_ModelMatrixInstanceBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ModelMatrixInstanceBuffer);
    glNamedBufferData(m_ModelMatrixInstanceBuffer, modelMatrix_buffer_size, nullptr, GL_DYNAMIC_DRAW);
    std::cout << "\tglCreate : SSBO - Model Matrix Buffer\n";
    // PARTICLE BUFFERS
    std::cout << "\nParticles***\n";
    glCreateBuffers(1, &m_GpuBuffers.PositionBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GpuBuffers.PositionBuffer);
    glNamedBufferData(m_GpuBuffers.PositionBuffer, maxParticleBufferSize, nullptr, GL_DYNAMIC_DRAW);
    std::cout << "\tglCreate : SSBO - Position Buffer\n";
    glCreateBuffers(1, &m_GpuBuffers.VelocityBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GpuBuffers.VelocityBuffer);
    glNamedBufferData(m_GpuBuffers.VelocityBuffer, maxParticleBufferSize, nullptr, GL_DYNAMIC_DRAW);
    std::cout << "\tglCreate : SSBO - Velocity Buffer\n";
    glCreateBuffers(1, &m_GpuBuffers.LifetimeAndSizeBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GpuBuffers.LifetimeAndSizeBuffer);
    glNamedBufferData(m_GpuBuffers.LifetimeAndSizeBuffer, maxParticleBufferSize, nullptr, GL_DYNAMIC_DRAW);
    std::cout << "\tglCreate : SSBO - Lifecycle Buffer\n";
    glCreateBuffers(1, &m_GpuBuffers.ColorBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GpuBuffers.ColorBuffer);
    glNamedBufferData(m_GpuBuffers.ColorBuffer, maxParticleBufferSize, nullptr, GL_DYNAMIC_DRAW);
    std::cout << "\tglCreate : SSBO - Color Buffer\n";
    // SPLINE BUFFERS
    std::cout << "\nSplines***\n";
    glCreateBuffers(1, &m_SplineBuffer.ControlPointBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SplineBuffer.ControlPointBuffer);
    glNamedBufferData(m_SplineBuffer.ControlPointBuffer, maxControlPointBufferSize, nullptr, GL_STATIC_DRAW);
    std::cout << "\tglCreate : SSBO - Control Point Buffer\n";
    glCreateBuffers(1, &m_SplineBuffer.KnotBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SplineBuffer.KnotBuffer);
    glNamedBufferData(m_SplineBuffer.KnotBuffer, maxKnotBufferSize, nullptr, GL_STATIC_DRAW);
    std::cout << "\tglCreate : SSBO - Knot Buffer\n";

    GLint64 maxSSBOBlockSize = 0;
    glGetInteger64v(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxSSBOBlockSize);
    GLint maxSSBOBindings = 0;
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &maxSSBOBindings);
    std::cout << "\nSpec***\n"
        << "\nmax shader block size \t\t:" << maxSSBOBlockSize
        << "\nmax shader buffer binding \t:" << maxSSBOBindings
        << "\n";

    BufferLayout layout{
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float2, "TexCoord"},
        {ShaderDataType::Float,  "ID"},
        {ShaderDataType::Float3, "Normal"}
    };
    m_VAO.Bind();
    m_VAO.SetBufferLayout(layout);
    m_GpuBuffers.Positions = (glm::vec4*)glMapNamedBufferRange(m_GpuBuffers.PositionBuffer, 0, maxParticleBufferSize, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    m_GpuBuffers.Velocities = (glm::vec4*)glMapNamedBufferRange(m_GpuBuffers.VelocityBuffer, 0, maxParticleBufferSize, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    m_GpuBuffers.LifetimeAndSize = (glm::vec4*)glMapNamedBufferRange(m_GpuBuffers.LifetimeAndSizeBuffer, 0, maxParticleBufferSize, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    m_GpuBuffers.Color = (glm::vec4*)glMapNamedBufferRange(m_GpuBuffers.ColorBuffer, 0, maxParticleBufferSize, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    m_SplineBuffer.ControlPoints = (glm::vec4*)glMapNamedBufferRange(m_SplineBuffer.ControlPointBuffer, 0, maxControlPointBufferSize, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    m_SplineBuffer.Knots = (float*)glMapNamedBufferRange(m_SplineBuffer.KnotBuffer, 0, maxKnotBufferSize, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
}

Renderer::~Renderer()
{
    std::cout << "OpenGL Render : Shutdown\n";
    glUnmapNamedBuffer(m_GpuBuffers.PositionBuffer);
    glUnmapNamedBuffer(m_GpuBuffers.VelocityBuffer);
    glUnmapNamedBuffer(m_GpuBuffers.LifetimeAndSizeBuffer);
    glUnmapNamedBuffer(m_GpuBuffers.ColorBuffer);
    glUnmapNamedBuffer(m_SplineBuffer.ControlPointBuffer);
    glUnmapNamedBuffer(m_SplineBuffer.KnotBuffer);
    glDeleteBuffers(1, &m_VertexBuffer);
    std::cout << "glDelete : Vertex Buffer\n";
    glDeleteBuffers(1, &m_IndexBuffer);
    std::cout << "glDelete : Index Buffer\n";
    glDeleteBuffers(1, &m_MaterialBuffer);
    std::cout << "glDelete : SSBO - Material Buffer\n";
    glDeleteBuffers(1, &m_ModelMatrixInstanceBuffer);
    std::cout << "glDelete : SSBO - Model Buffer\n";
    std::cout << "Particles ***\n";
    glDeleteBuffers(1, &m_GpuBuffers.PositionBuffer);
    std::cout << "glDelete : SSBO - Position Buffer\n";
    glDeleteBuffers(1, &m_GpuBuffers.VelocityBuffer);
    std::cout << "glDelete : SSBO - Velocity Buffer\n";
    glDeleteBuffers(1, &m_GpuBuffers.LifetimeAndSizeBuffer);
    std::cout << "glDelete : SSBO - Lifecycle Buffer\n";
    glDeleteBuffers(1, &m_GpuBuffers.ColorBuffer);
    std::cout << "glDelete : SSBO - Color Buffer\n";
    std::cout << "Splines ***\n";
    glDeleteBuffers(1, &m_SplineBuffer.ControlPointBuffer);
    std::cout << "glDelete : SSBO - Control Point Buffer\n";
    glDeleteBuffers(1, &m_SplineBuffer.KnotBuffer);
    std::cout << "glDelete : SSBO - Knot Buffer\n";
}

uint32_t Renderer::AllocateRange(uint32_t size, std::vector<primitives::Range>& allocated_ranges, std::vector<primitives::Range>& free_ranges) const
{
    uint32_t currentOffset = -1;
    const uint32_t requestedSize = size;
    for (auto rangeIterator = free_ranges.begin(); rangeIterator != free_ranges.end(); rangeIterator++)
    {
        if (requestedSize <= rangeIterator->Size)
        {
            currentOffset = rangeIterator->Offset;
            primitives::Range allocatedRange{ currentOffset, requestedSize };
            allocated_ranges.push_back(allocatedRange);
            rangeIterator->Offset += requestedSize;
            rangeIterator->Size -= requestedSize;
            if (rangeIterator->Size == 0)
            {
                free_ranges.erase(rangeIterator);
            }
            return currentOffset;
        }
    }
    return currentOffset;
}

bool Renderer::DeallocateRange(const primitives::Range& range, std::vector<primitives::Range>& allocated_ranges, std::vector<primitives::Range>& free_ranges) const
{
    auto rangeIter = std::find(allocated_ranges.begin(), allocated_ranges.end(), range);
    if (rangeIter == allocated_ranges.end())
    {
        return false;
    }
    allocated_ranges.erase(rangeIter);
    auto it = free_ranges.begin();
    while (it != free_ranges.end() && it->Offset < range.Offset)
    {
        ++it;
    }
    free_ranges.insert(it, range);
    MergeFreeRanges(free_ranges);
    return true;
}

void Renderer::MergeFreeRanges(std::vector<primitives::Range>& free_ranges) const
{
    if (free_ranges.size() <= 1) return;
    for (std::size_t index = 0; index < free_ranges.size() - 1;)
    {
        auto& current = free_ranges[index];
        auto& next = free_ranges[index + 1];
        if (current.Offset + current.Size >= next.Offset)
        {
            current.Size = glm::max<uint32_t>(current.Size, (next.Offset + next.Size) - current.Offset);
            free_ranges.erase(free_ranges.begin() + (index + 1));
        }
        else
        {
            ++index;
        }
    }
}

void Renderer::DebugRanges(std::vector<primitives::Range>& allocated_ranges, std::vector<primitives::Range>& free_ranges) const
{
    std::sort(allocated_ranges.begin(), allocated_ranges.end());
    std::sort(free_ranges.begin(), free_ranges.end());

    std::cout << "-----------------------------------------------\n";
    std::cout << "Allocated Ranges:\n{\n";
    for (auto& range : allocated_ranges)
    {
        std::cout << "[" << range.Offset << ", " << (range.Offset + range.Size) << " ], ";
    }
    std::cout << "\n}\n";
    std::cout << "Free Ranges:\n{\n";
    for (auto& range : free_ranges)
    {
        std::cout << "[" << range.Offset << ", " << (range.Offset + range.Size) << " ], ";
    }
    std::cout << "\n}\n";
}

void Renderer::EnsureParticleAllocation(Scene* scene)
{
    auto particleView = scene->GetView<primitives::ParticleSystemInstance>();
    auto assetManager = scene->GetAssetManager();
    for (auto [entity, particleSystemInstance] : particleView.each())
    {
        auto& particleSystem = assetManager->m_ParticleSystems[particleSystemInstance.m_ConfigIndex];
        if (particleSystemInstance.m_Allocation.Offset == -1)
        {
            particleSystemInstance.m_Allocation.Offset = AllocateParticles(particleSystem.m_MaxNumParticles);
            particleSystemInstance.m_Allocation.Size = particleSystem.m_MaxNumParticles;
            DebugRanges(m_AllocatedParticleRanges, m_FreeParticleRanges);
        }
    }
}

void Renderer::ReinstantiateParticleInstances(Scene* scene)
{
    static std::mt19937_64 rng(std::random_device{}());
    auto assetManager = scene->GetAssetManager();
    //auto particleView = scene->GetView<primitives::ParticleSystemInstance>();
    //for (auto [entity, particleSystemInstance] : particleView.each())
    auto particleView = scene->GetView<primitives::ParticleSystemInstance, primitives::Transform>();
    for (auto [entity, particleSystemInstance, transform] : particleView.each())
    {
        const auto& worldPosition = transform.worldPosition;
        const auto& worldOrientation = transform.worldOrientation;
        auto& particleSystem = assetManager->m_ParticleSystems[particleSystemInstance.m_ConfigIndex];
        if (particleSystemInstance.m_Restart)
        {
            particleSystemInstance.m_AccumulatedTime = 0.0f;
            particleSystemInstance.m_NumParticles = 0;

            const glm::vec4 baseVelocity
            {
                particleSystem.m_EmitterInfo.m_VectorTwo.x * particleSystem.m_InitialSpeed,
                particleSystem.m_EmitterInfo.m_VectorTwo.y * particleSystem.m_InitialSpeed,
                particleSystem.m_EmitterInfo.m_VectorTwo.z * particleSystem.m_InitialSpeed,
                particleSystem.m_EmitterInfo.m_VectorTwo.w
            };
            if (particleSystem.m_EmitterInfo.m_Shape == primitives::EmitterShape::POINT)
            {
                std::uniform_real_distribution<float> rangesR(-1.0f, 1.0f);
                for (auto index = particleSystemInstance.m_Allocation.Offset; index <  (particleSystemInstance.m_Allocation.Offset + particleSystemInstance.m_Allocation.Size); index++)
                {
                    const glm::vec3 emitterPosition{ particleSystem.m_EmitterInfo.m_VectorOne };
                    glm::vec3 ir = worldPosition + emitterPosition;
                    m_GpuBuffers.Positions[index] = glm::vec4(ir, particleSystem.m_EmitterInfo.m_VectorOne.w);
                    if (particleSystem.m_EmitterInfo.m_Flags.m_RandomOrientation)
                    {
                        const auto v = glm::normalize(glm::vec3(rangesR(rng), rangesR(rng), rangesR(rng)));
                        m_GpuBuffers.Velocities[index] = glm::vec4(v, 0.0) * particleSystem.m_InitialSpeed;
                    }
                    else
                    {
                        m_GpuBuffers.Velocities[index] = baseVelocity;
                    }
                    m_GpuBuffers.LifetimeAndSize[index] = glm::vec4(0.0f, particleSystem.m_Duration, 0.0f, 1.0f);
                    m_GpuBuffers.Color[index] = glm::vec4(0.0f);
                }
            }

            if (particleSystem.m_EmitterInfo.m_Shape == primitives::EmitterShape::CIRCLE)
            {
                const auto radius = particleSystem.m_EmitterInfo.m_VectorOne.w;
                const auto center = glm::vec3(particleSystem.m_EmitterInfo.m_VectorOne);
                const auto normal = glm::vec3(particleSystem.m_EmitterInfo.m_VectorTwo);
                std::uniform_real_distribution<float> rangesR(-radius, radius);
                std::uniform_real_distribution<float> angle(-glm::pi<float>(), glm::pi<float>());
                glm::vec3 a{ 0 };
                if (glm::abs(normal.x) < 0.99f)
                {
                    a = glm::vec3(1, 0, 0);
                }
                else
                {
                    a = glm::vec3(0, 1, 0);
                }
                const auto u = glm::cross(normal, a);
                const auto v = glm::cross(normal, u);
                for (auto index = particleSystemInstance.m_Allocation.Offset; index < (particleSystemInstance.m_Allocation.Offset + particleSystemInstance.m_Allocation.Size); index++)
                {
                    const float randomAngle = angle(rng);
                    float radialDistribution = radius;
                    if (particleSystem.m_EmitterInfo.m_Flags.m_Fill)
                    {
                        radialDistribution = rangesR(rng);
                    }
                    float v1 = radialDistribution * glm::cos(randomAngle);
                    float v2 = radialDistribution * glm::sin(randomAngle);
                    const auto newPos = center + v1 * u + v2 * v;
                    const glm::vec3 ir = worldPosition + worldOrientation * newPos;
                    m_GpuBuffers.Positions[index] = glm::vec4(ir, radius);
                    m_GpuBuffers.Velocities[index] = baseVelocity;
                    m_GpuBuffers.LifetimeAndSize[index] = glm::vec4(0.0f, particleSystem.m_Duration, 0.0f, 1.0f);
                    m_GpuBuffers.Color[index] = glm::vec4(0.0f);
                }
            }

            if (particleSystem.m_EmitterInfo.m_Shape == primitives::EmitterShape::CYLINDER)
            {
                const auto radius = particleSystem.m_EmitterInfo.m_VectorOne.w;
                const auto center = glm::vec3(particleSystem.m_EmitterInfo.m_VectorOne);
                const auto height = particleSystem.m_EmitterInfo.m_VectorTwo.w;
                const auto normal = glm::normalize(glm::vec3(particleSystem.m_EmitterInfo.m_VectorTwo));
                std::uniform_real_distribution<float> rangesR(-radius, radius);
                std::uniform_real_distribution<float> angleDist(-glm::pi<float>(), glm::pi<float>());
                std::uniform_real_distribution<float> heightDist(0.0f, 1.0f);
                float v1 = 0.0f;
                float v2 = 0.0f;
                float h1 = 0.0f;
                glm::vec3 a{ 0 };
                if (glm::abs(normal.x) < 0.99f)
                {
                    a = glm::vec3(1, 0, 0);
                }
                else
                {
                    a = glm::vec3(0, 1, 0);
                }
                auto u = glm::cross(normal, a);
                auto v = glm::cross(normal, u);
                for (auto index = particleSystemInstance.m_Allocation.Offset; index < (particleSystemInstance.m_Allocation.Offset + particleSystemInstance.m_Allocation.Size); index++)
                {
                    if (particleSystem.m_EmitterInfo.m_Flags.m_Fill)
                    {
                        v1 = rangesR(rng);
                        v2 = rangesR(rng);
                        h1 = height * heightDist(rng);
                    }
                    else
                    {
                        float randomAngle = angleDist(rng);
                        v1 = radius * glm::cos(randomAngle);
                        v2 = radius * glm::sin(randomAngle);
                    }
                    auto newPos = center + v1 * u + v2 * v + h1 * normal;
                    const glm::vec3 ir = worldPosition + worldOrientation * newPos;
                    m_GpuBuffers.Positions[index] = glm::vec4(ir, radius);
                    m_GpuBuffers.Velocities[index] = baseVelocity;
                    m_GpuBuffers.LifetimeAndSize[index] = glm::vec4(0.0f, particleSystem.m_Duration, 0.0f, 1.0f);
                    m_GpuBuffers.Color[index] = glm::vec4(0.0f);
                }
            }

            if (particleSystem.m_EmitterInfo.m_Shape == primitives::EmitterShape::CONE)
            {
                const auto halfAngle = particleSystem.m_EmitterInfo.m_VectorOne.w;
                const auto apex = glm::vec3(particleSystem.m_EmitterInfo.m_VectorOne);
                const auto height = particleSystem.m_EmitterInfo.m_VectorTwo.w;
                const auto normal = glm::vec3(particleSystem.m_EmitterInfo.m_VectorTwo);
                std::uniform_real_distribution<float> angleDist(-halfAngle, halfAngle);
                std::uniform_real_distribution<float> radiusDist(-glm::pi<float>(), glm::pi<float>());
                std::uniform_real_distribution<float> heightDist(0, 1);
                glm::vec3 a{ 0 };
                if (glm::abs(normal.x) < 0.99f)
                {
                    a = glm::vec3(1, 0, 0);
                }
                else
                {
                    a = glm::vec3(0, 1, 0);
                }
                const auto u = glm::normalize(glm::cross(normal, a));
                const auto v = glm::normalize(glm::cross(normal, u));
                for (auto index = particleSystemInstance.m_Allocation.Offset; index < (particleSystemInstance.m_Allocation.Offset + particleSystemInstance.m_Allocation.Size); index++)
                {
                    float randomAngle = halfAngle;
                    float randomHeight = height;
                    float planeAngle = radiusDist(rng);
                    if (particleSystem.m_EmitterInfo.m_Flags.m_Fill)
                    {
                        randomAngle = angleDist(rng);
                        planeAngle = radiusDist(rng);
                        randomHeight = height * heightDist(rng);
                    }
                    float randomRadius = randomHeight * glm::tan(glm::radians(randomAngle));
                    auto axisPoint = apex + normal * randomHeight;
                    const glm::vec3 newPos = axisPoint + randomRadius * (glm::cos(planeAngle) * u + glm::sin(planeAngle) * v);
                    auto worldVelocity = glm::normalize(newPos - apex) * particleSystem.m_InitialSpeed;
                    const glm::vec3 ir = worldPosition + worldOrientation * newPos;
                    m_GpuBuffers.Positions[index] = glm::vec4(ir, halfAngle);
                    m_GpuBuffers.Velocities[index] = glm::vec4(worldVelocity, particleSystem.m_EmitterInfo.m_VectorTwo.w);
                    m_GpuBuffers.LifetimeAndSize[index] = glm::vec4(0.0f, particleSystem.m_Duration, 0.0f, 1.0f);
                    m_GpuBuffers.Color[index] = glm::vec4(0.0f);
                }
            }

            if (particleSystem.m_EmitterInfo.m_Shape == primitives::EmitterShape::SPHERE)
            {
                const auto radius = particleSystem.m_EmitterInfo.m_VectorOne.w;
                const auto center = glm::vec3(particleSystem.m_EmitterInfo.m_VectorOne);
                std::uniform_real_distribution<float> dist01(0.0, 1.0);
                for (auto index = particleSystemInstance.m_Allocation.Offset; index < (particleSystemInstance.m_Allocation.Offset + particleSystemInstance.m_Allocation.Size); index++)
                {
                    float u = 1.0f;
                    if (particleSystem.m_EmitterInfo.m_Flags.m_Fill)
                    {
                        u = dist01(rng);
                    }
                    float u1 = dist01(rng);
                    float u2 = dist01(rng);
                    // Correct 3D radial distribution
                    float d = std::cbrt(u * (radius * radius * radius));
                    // Uniform direction on sphere
                    float theta = 2.0 * glm::pi<float>() * u1;
                    float z = 2.0 * u2 - 1.0;     // cos(phi)
                    float s = std::sqrt(1.0 - z * z);
                    float x = d * s * std::cos(theta);
                    float y = d * s * std::sin(theta);
                    float zc = d * z;
                    const glm::vec3 randomOffset(x, y, zc);
                    const glm::vec3 newPos = center + randomOffset;
                    const glm::vec3 ir = worldPosition + worldOrientation * newPos;
                    const glm::vec3 worldVelocity = glm::normalize(randomOffset) * particleSystem.m_InitialSpeed;
                    m_GpuBuffers.Positions[index] = glm::vec4(ir, radius);
                    m_GpuBuffers.Velocities[index] = glm::vec4(worldVelocity, particleSystem.m_EmitterInfo.m_VectorTwo.w);
                    m_GpuBuffers.LifetimeAndSize[index] = glm::vec4(0.0f, particleSystem.m_Duration, 0.0f, 1.0f);
                    m_GpuBuffers.Color[index] = glm::vec4(0.0f);
                }
            }
            particleSystemInstance.m_Restart = false;
        }
    }
}

void Renderer::UpdateGPUParticleStates(Scene* scene)
{
    const float fixedUpdateInterval = 1.0f / 60.0f;
    auto particleView = scene->GetView<primitives::ParticleSystemInstance>();
    auto assetManager = scene->GetAssetManager();
    for (auto [entity, particleSystemInstance] : particleView.each())
    {
        auto& particleSystem = assetManager->m_ParticleSystems[particleSystemInstance.m_ConfigIndex];
        if (particleSystemInstance.m_IsEnabled)
        {
            auto& shaderHandle = particleSystem.m_ShaderHandle;
            auto& shader = assetManager->GetAsset<Shader>(shaderHandle);

            shader.Bind();
            auto platformHandle = shader.GetHandle();
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GpuBuffers.PositionBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferIndices::GPU_POSITION_BUFFER, m_GpuBuffers.PositionBuffer);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GpuBuffers.VelocityBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferIndices::GPU_VELOCITY_BUFFER, m_GpuBuffers.VelocityBuffer);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GpuBuffers.LifetimeAndSizeBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferIndices::GPU_LIFETIME_AND_SIZE_BUFFER, m_GpuBuffers.LifetimeAndSizeBuffer);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GpuBuffers.ColorBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferIndices::GPU_COLOR_BUFFER, m_GpuBuffers.ColorBuffer);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SplineBuffer.ControlPointBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferIndices::GPU_CONTROL_POINT_BUFFER, m_SplineBuffer.ControlPointBuffer);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SplineBuffer.KnotBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferIndices::GPU_KNOT_BUFFER, m_SplineBuffer.KnotBuffer);

            GLuint deltaLoc = glGetUniformLocation(platformHandle, "delta");
            GLuint uTimeLoc = glGetUniformLocation(platformHandle, "uTime");
            GLuint bufferOffsetLoc = glGetUniformLocation(platformHandle, "bufferOffset");
            GLuint numPtsLoc = glGetUniformLocation(platformHandle, "numParticles");
            GLuint colorOneLoc = glGetUniformLocation(platformHandle, "colorOne");
            GLuint colorTwoLoc = glGetUniformLocation(platformHandle, "colorTwo");
            GLuint colorThreeLoc = glGetUniformLocation(platformHandle, "colorThree");

            glUniform1f(deltaLoc, fixedUpdateInterval);
            glUniform1f(uTimeLoc, particleSystemInstance.m_AccumulatedTime);
            glUniform1ui(bufferOffsetLoc, particleSystemInstance.m_Allocation.Offset);
            glUniform1ui(numPtsLoc, particleSystemInstance.m_NumParticles);
            glm::vec3 colorOne = glm::vec3{ 1.0, 0.998, 0.696 };
            glm::vec3 colorTwo = glm::vec3{ 0.955, 0.918, 0.243 };
            glm::vec3 colorThree = glm::vec3{ 0.986, 0.845, 0.370 };

            glUniform3f(colorOneLoc, colorOne.x, colorOne.y, colorOne.z);
            glUniform3f(colorTwoLoc, colorTwo.x, colorTwo.y, colorTwo.z);
            glUniform3f(colorThreeLoc, colorThree.x, colorThree.y, colorThree.z);

            glDispatchCompute(particleSystemInstance.m_NumParticles, 1, 1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);
            particleSystemInstance.m_AccumulatedTime += fixedUpdateInterval;
            if((particleSystemInstance.m_NumParticles + particleSystem.m_ParticleRate) <= particleSystemInstance.m_Allocation.Size)
            {
                particleSystemInstance.m_NumParticles += particleSystem.m_ParticleRate;
            }
        }
    }
}

void Renderer::UploadMaterialData(const RenderCommand& cmd, AssetManager& asset_manager) const
{
    auto& shader = asset_manager.GetShader(cmd.m_ShaderHandle);
    auto& materialGroup = asset_manager.GetMaterial(cmd.m_MaterialHandle);
    auto& newMaterialGroup = asset_manager.GetNewMaterial(cmd.m_MaterialHandle);
    auto shaderHandlePlatform = shader.GetHandle();
    shader.Bind();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_MaterialBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_MaterialBuffer);
    glShaderStorageBlockBinding(shaderHandlePlatform, 0, 0);

    /*/
    for (auto index = 0; index < materialGroup.size(); index++)
    {
        auto& material = materialGroup[index];
        glm::vec4 KaNs{ 0 };
        glm::vec4 KdNi{ 0 };
        glm::vec4 KsD{ 0 };
        glm::vec4 KeIllum{ 0 };
        for (auto& materialComponent : material.m_Uniforms1f)
        {
            auto& name = materialComponent.first;
            auto& data = materialComponent.second;
            if (name == "Ns")
            {
                KaNs[3] = data.x;
            }
            if (name == "Ni")
            {
                KdNi[3] = data.x;
            }
            if (name == "d")
            {
                KsD[3] = data.x;
            }
            if (name == "illum")
            {
                KeIllum[3] = data.x;
            }
        }
        for(auto& materialComponent : material.m_Uniforms3f)
        {
            auto& name = materialComponent.first;
            auto& data = materialComponent.second;
            if (name == "Kd")
            {
                KaNs[0] = data.x;
                KaNs[1] = data.y;
                KaNs[2] = data.z;
            }
            if (name == "Ka")
            {
                KdNi[0] = data.x;
                KdNi[1] = data.y;
                KdNi[2] = data.z;
            }
            if (name == "Ks")
            {
                KsD[0] = data.x;
                KsD[1] = data.y;
                KsD[2] = data.z;;
            }
            if (name == "Ke")
            {
                KeIllum[0] = data.x;
                KeIllum[1] = data.y;
                KeIllum[2] = data.z;
            }
        }
        MTLMaterial tempMaterial{ KaNs, KdNi, KsD, KeIllum };
        GLint materialOffset = index * sizeof(MTLMaterial);
        glNamedBufferSubData(m_MaterialBuffer, materialOffset, sizeof(MTLMaterial), &tempMaterial);
    }
    /**/
    for (auto index = 0; index < newMaterialGroup.size(); index++)
    {
        auto& newMaterial = newMaterialGroup[index];
        GLuint materialOffset = index * sizeof(MTLMaterial);
        glNamedBufferSubData(m_MaterialBuffer, materialOffset, sizeof(MTLMaterial), &newMaterial);
    }
    /**/
}

void Renderer::UploadUniformData(const RenderCommand& cmd, AssetManager& asset_manager) const
{
    auto& shader = asset_manager.GetShader(cmd.m_ShaderHandle);
    auto shaderHandlePlatform = shader.GetHandle();
    shader.Bind();

    for (auto& uniformFloat : cmd.m_UniformBuffer.m_FloatMap)
    {
        auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformFloat.first.c_str());
        glUniform1f(uniformLocation, uniformFloat.second);
    }
    for (auto& uniformFloat2 : cmd.m_UniformBuffer.m_Float2Map)
    {
        auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformFloat2.first.c_str());
        auto x = uniformFloat2.second.x;
        auto y = uniformFloat2.second.y;
        glUniform2f(uniformLocation, x, y);
    }
    for (auto& uniformFloat3 : cmd.m_UniformBuffer.m_Float3Map)
    {
        auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformFloat3.first.c_str());
        auto x = uniformFloat3.second.x;
        auto y = uniformFloat3.second.y;
        auto z = uniformFloat3.second.z;
        glUniform3f(uniformLocation, x, y, z);
    }
    for (auto& uniformFloat4 : cmd.m_UniformBuffer.m_Float4Map)
    {
        auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformFloat4.first.c_str());
        auto x = uniformFloat4.second.x;
        auto y = uniformFloat4.second.y;
        auto z = uniformFloat4.second.z;
        auto w = uniformFloat4.second.w;
        glUniform4f(uniformLocation, x, y, z, w);
    }
    for (auto& uniformMat4 : cmd.m_UniformBuffer.m_Mat4Map)
    {
        auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformMat4.first.c_str());
        auto mat = uniformMat4.second;
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Renderer::UploadUniformData(const ParticleCommand& cmd, AssetManager& asset_manager) const
{
    auto& shader = asset_manager.GetShader(cmd.m_ShaderHandle);
    auto shaderHandlePlatform = shader.GetHandle();
    shader.Bind();
    if(!cmd.m_UniformBuffer.m_FloatMap.empty())
    {
        for (auto& uniformFloat : cmd.m_UniformBuffer.m_FloatMap)
        {
            auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformFloat.first.c_str());
            glUniform1f(uniformLocation, uniformFloat.second);
        }
    }
    if(!cmd.m_UniformBuffer.m_Float2Map.empty())
    {
        for (auto& uniformFloat2 : cmd.m_UniformBuffer.m_Float2Map)
        {
            auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformFloat2.first.c_str());
            auto x = uniformFloat2.second.x;
            auto y = uniformFloat2.second.y;
            glUniform2f(uniformLocation, x, y);
        }
    }
    if(!cmd.m_UniformBuffer.m_Float3Map.empty())
    {
        for (auto& uniformFloat3 : cmd.m_UniformBuffer.m_Float3Map)
        {
            auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformFloat3.first.c_str());
            auto x = uniformFloat3.second.x;
            auto y = uniformFloat3.second.y;
            auto z = uniformFloat3.second.z;
            glUniform3f(uniformLocation, x, y, z);
        }
    }
    if(!cmd.m_UniformBuffer.m_Float4Map.empty())
    {
        for (auto& uniformFloat4 : cmd.m_UniformBuffer.m_Float4Map)
        {
            auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformFloat4.first.c_str());
            auto x = uniformFloat4.second.x;
            auto y = uniformFloat4.second.y;
            auto z = uniformFloat4.second.z;
            auto w = uniformFloat4.second.w;
            glUniform4f(uniformLocation, x, y, z, w);
        }
    }
    if(!cmd.m_UniformBuffer.m_Mat4Map.empty())
    {
        for (auto& uniformMat4 : cmd.m_UniformBuffer.m_Mat4Map)
        {
            auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformMat4.first.c_str());
            auto mat = uniformMat4.second;
            glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));
        }
    }
}

void Renderer::DrawBuffer(std::vector<RenderCommand>& command_queue, VertexBuffer& vertex_buffer, AssetManager& asset_manager) const
{
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    UploadBuffer(vertex_buffer);
    for (auto& cmd : command_queue)
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        auto enableBits = cmd.m_EnableBits;
        if (enableBits & 0x1)
        {
            glDisable(GL_CULL_FACE);
        }
        if (enableBits & 0x10)
        {
            glDisable(GL_DEPTH_TEST);
        }
        DrawInstanced(cmd, asset_manager);
    }
}

void Renderer::DrawInstanced(const RenderCommand& cmd, AssetManager& asset_manager, uint32_t instance_count) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glUseProgram(0);
    UploadMaterialData(cmd, asset_manager);
    UploadUniformData(cmd, asset_manager);
    auto viewportX = cmd.m_Viewport[0];
    auto viewportY = cmd.m_Viewport[1];
    auto viewportW = cmd.m_Viewport[2];
    auto viewportH = cmd.m_Viewport[3];
    glViewport(viewportX, viewportY, viewportW, viewportH);

    if (!(cmd.m_FrameBufferHandle.m_LWORD == U64_DEF || cmd.m_FrameBufferHandle.m_LWORD == 0))
    {
        auto& frameBuffer = asset_manager.GetAsset<unsigned int>(cmd.m_FrameBufferHandle);
        auto& texture = asset_manager.GetAsset<TextureBase<GL_Texture>>(cmd.m_FrameBufferHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glBindTexture(GL_TEXTURE_2D, texture.m_Texture);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
    }
    if (!(cmd.m_TextureHandle.m_LWORD == U64_DEF || cmd.m_TextureHandle.m_LWORD == 0))
    {
        auto& texture = asset_manager.GetAsset<TextureBase<GL_Texture>>(cmd.m_TextureHandle);
        glBindTextureUnit(0, texture.m_Texture);
    }
    glDrawArraysInstanced((GLenum)cmd.m_PrimitiveType, (GLint)cmd.m_IndexBufferOffset, (GLsizei)cmd.m_IndexCount, instance_count);
}

void Renderer::DrawBufferInstanced(std::vector<RenderCommand>& command_queue, VertexBuffer& vertex_buffer, AssetManager& asset_manager)
{
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    UploadBuffer(vertex_buffer);
    m_InstanceGroups.clear();
    m_InstanceGroups.reserve(command_queue.size());
    m_InstanceGroupKeys.clear();
    m_InstanceGroupsMap.clear();

    for (auto& renderCommand : command_queue)
    {
        /**/
        const auto& indexCount = renderCommand.m_IndexCount;
        size_t index = 0;
        while (index < m_InstanceGroupKeys.size() && m_InstanceGroupKeys[index] != indexCount)
        {
            ++index;
        }
        
        if (index == m_InstanceGroupKeys.size())
        {
            m_InstanceGroupKeys.push_back(indexCount);
            m_InstanceGroups.emplace_back();
        }
        m_InstanceGroups[index].reserve(command_queue.size());
        m_InstanceGroups[index].push_back(std::move(renderCommand));
        /**/
        /*/
        m_InstanceGroupsMap[renderCommand.m_IndexCount].push_back(
            std::move(renderCommand)
        );
        /**/
    }
    GLintptr currentBufferOffset{ 0 };
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ModelMatrixInstanceBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ModelMatrixInstanceBuffer);
    for (auto groupIndex = 0; groupIndex < m_InstanceGroups.size(); groupIndex++)
    {
        auto& group = m_InstanceGroups[groupIndex];
        uint64_t glDisableBits{ 0 };
        AssetHandle shaderHandle{ 0,0 };
        AssetHandle materialHandle{ 0, 0 };
        std::vector<glm::mat4> tempBuffer(group.size());
        for (auto cmdIndex = 0; cmdIndex < group.size(); cmdIndex++)
        {
            auto& cmd = group[cmdIndex];
            shaderHandle = cmd.m_ShaderHandle;
            materialHandle = cmd.m_MaterialHandle;
            auto& uniformBuffer = cmd.m_UniformBuffer;
            auto& modelMatrix = uniformBuffer.m_Mat4Map["Model"];
            tempBuffer[cmdIndex] = modelMatrix;
            glDisableBits = cmd.m_EnableBits;
        }

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        if (glDisableBits & 0x1)
        {
            glDisable(GL_CULL_FACE);
        }
        if (glDisableBits & 0x10)
        {
            glDisable(GL_DEPTH_TEST);
        }
        auto& shader = asset_manager.GetShader(shaderHandle);
        auto& materialGroup = asset_manager.GetMaterial(materialHandle);
        auto shaderHandlePlatform = shader.GetHandle();
        shader.Bind();

        glNamedBufferSubData(m_ModelMatrixInstanceBuffer, 0, sizeof(glm::mat4) * tempBuffer.size(), tempBuffer.data());
        auto& front = group.front();
        uint32_t instanceCount = static_cast<uint32_t>(group.size());
        DrawInstanced(front, asset_manager, instanceCount);
        currentBufferOffset += sizeof(glm::mat4) * tempBuffer.size();
    }
    /*/
    for (auto& group : m_InstanceGroupsMap)
    {
        m_ModelMatrices.clear();
        m_ModelMatrices.reserve(131072);
        uint64_t glDisableBits{ 0 };
        AssetHandle shaderHandle{ 0,0 };
        AssetHandle materialHandle{ 0, 0 };
        for (auto& cmd : group.second)
        {
            shaderHandle = cmd.m_ShaderHandle;
            materialHandle = cmd.m_MaterialHandle;
            auto& uniformBuffer = cmd.m_UniformBuffer;
            auto& modelMatrix = uniformBuffer.m_Mat4Map["Model"];
            m_ModelMatrices.push_back(modelMatrix);
            glDisableBits = cmd.m_EnableBits;
        }

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        if (glDisableBits & 0x1)
        {
            glDisable(GL_CULL_FACE);
        }
        if (glDisableBits & 0x10)
        {
            glDisable(GL_DEPTH_TEST);
        }
        auto& shader = asset_manager.GetShader(shaderHandle);
        auto& materialGroup = asset_manager.GetMaterial(materialHandle);
        auto shaderHandlePlatform = shader.GetHandle();
        shader.Bind();

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ModelMatrixInstanceBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ModelMatrixInstanceBuffer);
        glNamedBufferSubData(m_ModelMatrixInstanceBuffer, 0, sizeof(glm::mat4) * m_ModelMatrices.size(), m_ModelMatrices.data());
        auto& front = group.second.front();
        uint32_t instanceCount = static_cast<uint32_t>(group.second.size());
        DrawInstanced(front, asset_manager, instanceCount);
    }
    /**/
}

void Renderer::DrawParticles(std::vector<ParticleCommand>& command_queue, AssetManager& asset_manager) const
{
    if (!command_queue.empty())
    {
        glEnable(GL_PROGRAM_POINT_SIZE);
        for (auto& particleCommand : command_queue)
        {
            uint32_t& bufferOffset = particleCommand.m_BufferOffset;
            uint32_t& numParticles = particleCommand.m_NumParticles;
            UploadUniformData(particleCommand, asset_manager);
            if (!(particleCommand.m_TextureHandle.m_LWORD == U64_DEF || particleCommand.m_TextureHandle.m_LWORD == 0))
            {
                auto& texture = asset_manager.GetAsset<TextureBase<GL_Texture>>(particleCommand.m_TextureHandle);
                glBindTextureUnit(0, texture.m_Texture);
            }
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GpuBuffers.PositionBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_GpuBuffers.PositionBuffer);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GpuBuffers.VelocityBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_GpuBuffers.VelocityBuffer);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GpuBuffers.LifetimeAndSizeBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_GpuBuffers.LifetimeAndSizeBuffer);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GpuBuffers.ColorBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_GpuBuffers.ColorBuffer);
            glDrawArraysInstanced(GL_POINTS, bufferOffset, 1, numParticles);
        }
        glDisable(GL_PROGRAM_POINT_SIZE);
    }
}

void Renderer::SetActiveCamera(std::shared_ptr<Camera> camera)
{
    m_pActiveCamera = camera;
}

void Renderer::OnUpdate(float ts)
{
    m_Ts = ts;
}

void Renderer::BeginFrame()
{
    Clear();
}

void Renderer::EndFrame()
{
    glFlush();
}
