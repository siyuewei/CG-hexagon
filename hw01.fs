#version 460 core
in vec3 ourColor;
out vec4 FragColor;

void main()
{
    // 计算点的位置，将点限制在半径为0.5的圆内
    vec2 center = vec2(0.5, 0.5);
    vec2 toCenter = gl_PointCoord - center;
    float distanceToCenter = length(toCenter);
    
    if (distanceToCenter > 0.5) {
        // 超出半径范围的部分不绘制
        discard;
    }
    
    // 内部的部分设置颜色
    FragColor = vec4(ourColor, 1.0);
}
