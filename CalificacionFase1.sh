#crearemos 5 discos del mismo tamaño
MKdisk -SIze::70 -path::"/home/archivos/fase1/" -name::"D1.dsk"
mkdisK -SiZe::71200 -path::"/home/archivos/fase1/" -name::"D2.dsk" +UNit::k
MKdisk -SIze::70 +uniT::M -PatH::"/home/archivos/fase1/" -NamE::"D3.dsk"
MKdisk -SIze::71200 -path::"/home/archivos/fase1/" -name::"D4.dsk" +unit::K
MKdisk -SIze::70 -path::"/home/archivos/fase1/" -name::"D5.dsk"

#vamos a verificar si hicieron validaciones con datos incorrectos
#primero el size tiene numero menor a 0, segudno el +unit tiene unidad desconocida
MKdisk -SIze::-60 -path::"/home/archivos/fase1/" -name::"D6.dsk"
MKdisk -SIze::60 +unit::Y -path::"/home/archivos/fase1/" -name::"D7.dsk"

#Eliminar discos, solo quedaran 3 discos
RMdisk -patH::"/home/archivos/fase1/D5.dsk"
RMdisk -patH::"/home/archivos/fase1/D4.dsk"

#Verificando validaciones con datos incorrectos en el rmdisk
#Deberia de dar error ya euq el disco 8 no existe
rmDiSK -path::"/home/archivos/fase1/D8.dsk"

#Crear 12 particiones del mismo tamaño en el primer disco con ajustes diferentes
#Todas son de 5 Mb (La extendida es de 50 Mb)
#Tipo:		Primaria	Primaria	Extendida	Logica	Logica	Logica	Logica	Logica	Logica	Logica	Logica Primaria
#Ajuste:	Primer 		Mejor		-			Peor	Peor	Peor	Peor	Peor	Peor	Peor	Mejor   Peor
#Nombre:	PRI 1		PRI 2		EXT			LOG 1	LOG 2	LOG 3	LOG 4	LOG 5	LOG 6	LOG 7	LOG8   PRI 3
fdisk -Size::5 +UniT::M -path::"/home/archivos/fase1/D1.dsk" \
      +type::P +fit::FF -name::"PRI1"
fdisk -path::"/home/archivos/fase 1/D1.dsk" -Size::5120 \
	  +fit::BF -name::"PRI2"
fdisk -path::"/home/archivos/fase1/D1.dsk" +type::E -name::"EXT" -Size::51200
fdisk +type::L -Size::5120 +Unit::K -path::"/home/archivos/fase1/D1.dsk" -name::"LOG1"
fdisK -name::"LOG2" -Size::5120 -path::"/home/archivos/fase1/D1.dsk" +type::L
fdisk +type::L -path::"/home/archivos/fase1/D1.dsk" -name::"LOG3" -Size::5120 +uniT::K
fdiSk +type::L -Size::5120 +unIt::K -PAth::"/home/archivos/fase1/D1.dsk" -name::"LOG4"
fdisk +type::L -Size::5120 +unit::K -path::"/home/archivos/fase1/D1.dsk" -name::"LOG5"
FdisK -Size::5120 -NamE::"LOG6" +UNit::K -path::"/home/archivos/fase1/D1.dsk" +type::L
fdIsk +type::L -siZe::5120 +unit::K -path::"/home/archivos/fase1/D1.dsk" -name::"LOG7"
fdIsk +type::L -siZe::5120 +unit::K -path::"/home/archivos/fase1/D1.dsk" -name::"LOG8" +fit::BF
fdisk +unit::B -path::"/home/archivos/fase1/D1.dsk" -Size::5242880 -name::"PRI3"

#Crearemos las mismas 12 particiones en el tercer disco
fdisk -Size::5 +UniT::M -path::"/home/archivos/fase1/D3.dsk" \
     +type::P +fit::FF -name::"PRI1"
fdisk -path::"/home/archivos/fase1/D3.dsk" -Size::5120 \
	 +fit::BF -name::"PRI2"
FdisK -path::"/home/archivos/fase1/D3.dsk" +type::E -name::"EXT" -Size::51200
fdisk +type::L -Size::5120 +Unit::K -path::"/home/archivos/fase1/D3.dsk" -name::"LOG1"
fdisK -name::"LOG 2" -Size::5120 -path::"/home/archivos/fase1/D3.dsk" +type::L
fdisk +type::L -path::"/home/archivos/fase1/D3.dsk" -name::"LOG3" -Size::5120 +uniT::K
fdiSk -Size::5120 +unIt::K -PAth::"/home/archivos/fase1/D3.dsk" -name::"LOG4" +type::L
fdisk +type::L -Size::5120 +unit::K -path::"/home/archivos/fase1/D3.dsk" -name::"LOG5"
FdisK -Size::5120 -NamE::"LOG6" +UNit::K -path::"/home/archivos/fase1/D3.dsk" +type::L
fdIsk +type::L -siZe::5120 +unit::K -path::"/home/archivos/fase1/D3.dsk" -name::"LOG7"
fdIsk +type::L -siZe::5120 +unit::K -path::"/home/archivos/fase1/D3.dsk" -name::"LOG8" +fit::BF
fdisk +unit::B -path::"/home/archivos/fase1/D3.dsk" -Size::5242880 -name::"PRI3"

#Verificar generacion de ids al montar
#Debería generar los ids: vda1, vdb1, vda2, vdb2
#vda para el d3.dsk y vdb para d1.dsk
mouNt -NamE::"PRI3"  -path::"/home/archivos/fase1/D3.dsk"
moUnt -PAth::"/home/archivos/fase1/D1.dsk" -name::"PRI3"
mOunt -name::"PRI2" -path::"/home/archivos/fase1/D3.dsk"
mouNt -path::"/home/archivos/fase1/D1.dsk" -name::"PRI2"

#Validaciones al montar, no deberia cargarlos
moUnt -name::PX -path::"/home/archivos/fase1/D3.dsk"
moUNt -path::"/home/archivos/fase1/DX.dsk" -name::"PRI3"

#Listar las particiones montadas
mouNt
#SAlida es la siguiente
#id::vda1 -path::"/home/archivos/fase 1/D3.dsk" -NamE::"PRI 3"
#id::vdb1 -PAth::"/home/archivos/fase 1/D1.dsk" -name::"PRI 3"
#id::vda2 -path::"/home/archivos/fase 1/D3.dsk" -name::"PRI 2"
#id::vdb2 -path::"/home/archivos/fase 1/D1.dsk" -name::"PRI 2"

#Reporte de las particiones del disco
#Debe crear la carpeta de reportes
rep -name::"disk" -path::"/home/archivos/fase1/reportes/Reporte1Disco3.jpg" -id::vda1
rep -path::"/home/archivos/fase1/reportes/Reporte1Disco1.jpg" -id::vdb1 -name::"disk"

#Vericamos la eliminación
#Elimina la primaria 2, logica 1, logica 2 y logica 5 del disco 1
#Queda así: P1 EXT L2 L3 L6 L7 L8 P3
fdisk +delete::Fast -name::"PRI2" -path::"/home/archivos/fase1/D1.dsk"
fdisk -path::"/home/archivos/fase1/D1.dsk" -name::"LOG1" +delete::FasT
fDisk -name::"LOG4" +delete::FaST -PAth::"/home/archivos/fase1/D1.dsk"
fdisK -path::"/home/archivos/fase1/D1.dsk" -NamE::"LOG5" +delete::FAsT

#Tambien elimina la partición extendida y la primaria 1 del disco 3
#Verificar que no queden EBRs
#Queda así: P2 P3
fDisk -DELete::Full -Name::"EXT" -path::"/home/archivos/fase1/D3.dsk" +Delete::Full
fdisk +Delete::Full -name::"PRI1" +Delete::Full -path::"/home/archivos/fase1/D3.dsk"

#Reporte con particiones eliminadas
rep -id::vda1 -name::"disk" -path::"/home/archivos/fase1/reportes/Reporte2Disco3.jpg"
rep -path::"/home/archivos/fase1/reportes/Reporte2Disco1.jpg" -name::"disk" -id::vdb1

#Teoria de particiones, (solo una extendida) debería dar error
fdisk -Size::2048 -path::"/home/archivos/fase1/D1.dsk" +type::E -name::"EXT2"

#Verificar primer ajuste creando de nuevo las particiones
#Debe quedar de la siguiente forma:
#Disco1: P1 P2 EXT L1 L2 L3 L45 L6 L7 L8 P3
fdiSk +unit::M -Size::5 -path::"/home/archivos/fase1/D1.dsk" +type::P +fit::FF -name::"PRI2"
fdisk -path::"/home/archivos/fase1/D1.dsk" -Size::10240 +type::L -name::"LOG45" +unit::K
fdisk -path::"/home/archivos/fase1/D1.dsk" -Size::5120 +type::L -name::"LOG1" +unit::K
#Disco 3: P2 EXT L1 P1 P3
fdisk +type::E -path::"/home/archivos/fase1/D3.dsk" -Size::10240 -name::"EXT"
fdiSk -name::"LOG1" +unit::K -path::"/home/archivos/fase1/D3.dsk" +type::L -Size::5120
fdisk -Size::10240 -name::"PRI1" -path::"/home/archivos/fase1/D3.dsk" +type::P

#Reporte de las particiones del disco
Rep -Path::"/home/archivos/fase1/reportes/Reporte3Disco3.jpg" -id::vda1 -name::"disk"
rep -name::"disk" -path::"/home/archivos/fase1/reportes/Reporte3Disco2.jpg" -id::vdb1

#Validaciones al crear o quitar espacio
#No deberia ejecutar estas instrucciones
#(porque no hay espacio después)
fdisk -path::"/home/archivos/fase1/D1.dsk" -name::"PRI1" +add::1
#(porque no debe quedar espacio negativo) debe mostrar mensaje de cuanto puede quitarle
fdisk +add::-20 +unit::M -path::"/home/archivos/fase1/D1.dsk" -name::"PRI1" +unit::M
#(Porque no hay suficiente espacio)
fdisk +unit::M -path::"/home/archivos/fase1/D1.dsk" -name::"LOG7" +add::15

#Quita y agrega 1 mb de la primer particion
fdisk +unit::M -path::"/home/archivos/fase1/D1.dsk" -name::"PRI1" +add::-1
rep -name::"Mbr" -path::"/home/archivos/fase1/reportes/Reporte1MBRDisco1.jpg" -id::vdb2
fdisk +unit::M -path::"/home/archivos/fase1/D1.dsk" -name::"PRI1" +add::1
rep -name::"Mbr" -path::"/home/archivos/fase1/reportes/Reporte2MBRDisco1.jpg" -id::vdb2
#Le quita 1 mb a la partición logica
fdisk +unit::M -path::"/home/archivos/fase1/D1.dsk" -name::"LOG45" +add::-1
rep -name::"Mbr" -path::"/home/archivos/fase1/reportes/Reporte3MBRDisco1.jpg" -id::vdb2

#desmontar una lista de ids
umount -id1::vda1 -id2::vdb2 -id::vda2
#para corroborar que desmonto hacer un mount para listar las particiones montadas
mount
#Salida es la siguiente
#id::vdb1 -PAth::"/home/archivos/fase 1/D1.dsk" -name::"PRI 3"
