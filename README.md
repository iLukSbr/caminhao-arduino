# Caminhão Guincho com Arduino

Autor: Lucas Yukio Fukuda Matsumoto

Códigos de um caminhão guincho de Arduino com controles RF e infravermelho, Projeto Final da disciplina EEX11 – INTRODUÇÃO À PRÁTICAS DE LABORATÓRIO EM ELETRICIDADE E ELETRÔNICA, da Universidade Tecnológica Federal do Paraná.

<p align="center">
  <iframe width="560" height="315" src="https://www.youtube.com/embed/skqn2S_cQes" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
</p>

O Arduino Uno recebe o shield ponte H 4WD L293D, o Micro recebe os receptores, e o Nano os emissores.

A miniatura de caminhão guincho é controlada tanto por controle infravermelho quanto por joystick de radiofrequência simultaneamente, tendo como base o kit carrinho de acrílico para Arduino com shield disponível comercialmente. Ele possui tração nas 4 rodas, faróis de LED, setas de LED sequenciais com som “tick” de piscada, luz de freio LED, buzina, braço robótico com garra, sensor de obstáculos, carregamento de baterias por micro USB ou conector P4 12 V, baterias de Li-íon e carroceria decorativa de papelão.

O software embarcado está em um Arduino Uno R3 e em um Arduino Micro. Ambos se comunicam e enviam comandos entre si via I2C, então a relação é igual, não há escravidão de microcontrolador. O Uno está responsável pelas rodas e sensor de obstáculos ultrassônico, e o Micro pelos controles infravermelho e RF e dos relés. São alimentados por 9 VCC. O Micro foi necessário porque o infravermelho e RF usam o mesmo clock do shield motor, então dava conflito e não funcionava.

O I²C é um protocolo de comunicação entre controladores que utiliza apenas dois fios, conectados ao serial data (SDA) e serial clock (SCL). Se forem conectados muitos, pode ser necessários resistores de pull up, entre o positivo e o SDA e SCL.

Há apenas um sensor de obstáculos ultrassônico, na frente, que fica girando por um servomotor e observando à frente do carrinho. Ele envia um pulso de onda que é refletido e gera um eco ao atingir algum objeto. Esse eco é captado pelo sensor, e calcula-se a distância pelo tempo que levou para retornar. Se há obstáculos a menos de 20 centímetros, o caminhão para automaticamente. O sensor e seu servomotor só são ativados caso o caminhão esteja se deslocando para a frente ou fazendo curvas enquanto anda para a frente.

O caminhão anda para frente e de ré, é capaz de realizar curvas e frear ou acelerar, girar o braço robótico para frente e para trás, para cima e para baixo e para a esquerda e para a direita, e acender/apagar as luzes conforme comandos no controle infravermelho. No joystick também é possível, mas não tem o controle das luzes nem de aceleração, devido à necessidade de adicionar muito mais botões e alavancas se fosse implementar tudo no novo controle. Então, os dois controles se completam e são capazes de operar simultaneamente. Abaixo estão os códigos e comandos de cada botão e controles.
  
Os LEDs das setas e faróis e as buzinas são controlados por Arduino com módulo de 4 relés alimentado diretamente por 5,5 VCC. A luz de freio tem um módulo de relé único, também alimentado por 5,5 VCC. Os faróis, luzes de freio e buzina são alimentados por 12,6 VCC, e os LEDs das setas sequenciais são alimentados por 5,5 VCC, devido a restrições de corrente e tensão elétrica dos resistores e transistores que foram utilizados nos circuitos. Todo os circuitos das setas sequenciais e buzinas foram confecionados pelos autores em placa de fenolite perfurada ilhada com cobre, sendo soldados nela. São controlados por módulos relés 5V, acionados pelo Arduino Micro. Os circuitos dos faróis foram soldados nas próprias pernas dos componentes, e isolados com fita isolante.

O pisca das setas tem um conjunto de transistores BC547 para acender sequencialmente, um 555 para piscar e um alto-falante para soltar o “tick” de 8 Ohms. A buzina é feita na nota lá por um circuito miniórgão com 555, com alto-falante de 16 Ohms, mas é possível trocar manualmente a nota musical da buzina.
A luz de freio, na parte traseira, acende quando a variável velocidade é zero.

O braço robótico é composto por peças de acrílico, que vieram desmontadas no kit, e foram montadas como um quebra-cabeças. São parafusadas com folga, lubrificadas com desengripante e tem servomotores. É capaz de pegar, erguer e carregar um objeto de até 100 gramas aproximadamente, de acordo com os testes. O controle dos servos é ligado a um módulo PWM para servomotores de 16 canais I2C, alimentado diretamente por 5,5 VCC.

Os motores das rodas são os amarelos de plástico até 6 V que vieram no kit, e o controle dos motores das rodas e do servomotor do sensor ultrassônico é por um shield 4WD L293D, alimentado diretamente por 5,5 VCC.

A alimentação de energia é por 3 baterias 18650 4,2 V de 3800 mAh cada de alta descarga, em série resultando 12,6 V, com proteção BMS de 20 A. O cabeamento tem um sistema com 3 tensões, de 5,5 VCC, 9 VCC e 12,6 VCC, cada um com seu conector P4, e regulados por módulos step ups e step downs. Tudo é identificado por cores, sendo positivo com cabos vermelhos, negativo com cabos pretos, e sinais e dados com cabos coloridos. É possível funcionar o carrinho enquanto está carregando, inclusive consegue utilizar a carga proveniente de fonte externa, seja via micro USB 5 VCC ou plugue P4 12 VCC. Há uma chave de 2 posições liga/desliga.
