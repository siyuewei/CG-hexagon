#version 460 core
in vec3 ourColor;
out vec4 FragColor;

void main()
{
    // ������λ�ã����������ڰ뾶Ϊ0.5��Բ��
    vec2 center = vec2(0.5, 0.5);
    vec2 toCenter = gl_PointCoord - center;
    float distanceToCenter = length(toCenter);
    
    if (distanceToCenter > 0.5) {
        // �����뾶��Χ�Ĳ��ֲ�����
        discard;
    }
    
    // �ڲ��Ĳ���������ɫ
    FragColor = vec4(ourColor, 1.0);
}
