#/bin/bash

sed -i .bak -e 's/Math\.h/MathLocal.h/' Actor.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' AudioSystem.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' BoneTransform.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' CameraComponent.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' CircleComponent.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' Collision.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' Font.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' Game.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' Math.cpp
sed -i .bak -e 's/Math\.h/MathLocal.h/' Mesh.cpp
sed -i .bak -e 's/Math\.h/MathLocal.h/' MatrixPalette.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' PhysWorld.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' Renderer.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' Shader.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' SoundEvent.h
sed -i .bak -e 's/Math\.h/MathLocal.h/' TargetComponent.cpp
sed -i .bak -e 's/Math\.h/MathLocal.h/' UIScreen.h

sed -i .bak -e 's/setParameterValue/setParameterByName/' SoundEvent.cpp
sed -i .bak -e 's/getParameterValue/getParameterByName/' SoundEvent.cpp
sed -i .bak -e 's/getLowLevelSystem/getCoreSystem/' AudioSystem.cpp

mv Math.h MathLocal.h

rm -f *.bak