 - ���� -
1, Data������ ����� ���ҽ����� ��´�
2. SDK/FEFileConverter/FEFileConverter.bat ������ ������� ���ҽ����� �������Ѵ� (��δ� bin/Resource)
    2.a ���� ���̴��ڵ�� �ڵ����� ������ ���ϹǷ� �������� ���Ŀ� �°� ����ִ´� (FEShaderSyntax.fes ����)
    2.b ���ο� ���׸����� ������������ �����ϰ�ʹٸ� �������� ���Ŀ� �°� ����ִ´� (FEMaterialSyntax.fem ����)
3. Scene ���� (���Ŀ� .scene ���ҽ� ������ ���� �е��� ���� �ʿ�)
    3.a �ַ���� UserScene������Ʈ�� IFEScenn Ŭ������ ��ӹ޴� Ŭ������ �ۼ��Ѵ�
    3.b �ۼ��� Ŭ������ virtual void Load() override; �Լ��� �߰��Ѵ�. (���� �ε��Ҷ� �� �Լ��� ȣ��ȴ�)
    3.c Load() �Լ����� ���� �����Ѵ� (������Ʈ �߰�, �� ������Ʈ�� ������Ʈ �߰�)
    3.d FEUserScene.cpp���� ���� LoadUserScene() �Լ��� pSM->AddScene(new SceneClass); �־��ش� (Add ������� Index �Ű���)
4. ���� �� �����Ѵ�