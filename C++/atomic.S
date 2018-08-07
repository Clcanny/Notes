
atomic:     file format elf64-x86-64


Disassembly of section .init:

0000000000000520 <_init>:
 520:	48 83 ec 08          	sub    $0x8,%rsp
 524:	48 8b 05 a5 0a 20 00 	mov    0x200aa5(%rip),%rax        # 200fd0 <__gmon_start__>
 52b:	48 85 c0             	test   %rax,%rax
 52e:	74 02                	je     532 <_init+0x12>
 530:	ff d0                	callq  *%rax
 532:	48 83 c4 08          	add    $0x8,%rsp
 536:	c3                   	retq   

Disassembly of section .plt:

0000000000000540 <.plt>:
 540:	ff 35 c2 0a 20 00    	pushq  0x200ac2(%rip)        # 201008 <_GLOBAL_OFFSET_TABLE_+0x8>
 546:	ff 25 c4 0a 20 00    	jmpq   *0x200ac4(%rip)        # 201010 <_GLOBAL_OFFSET_TABLE_+0x10>
 54c:	0f 1f 40 00          	nopl   0x0(%rax)

Disassembly of section .plt.got:

0000000000000550 <.plt.got>:
 550:	ff 25 a2 0a 20 00    	jmpq   *0x200aa2(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 556:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000000560 <_start>:
 560:	31 ed                	xor    %ebp,%ebp
 562:	49 89 d1             	mov    %rdx,%r9
 565:	5e                   	pop    %rsi
 566:	48 89 e2             	mov    %rsp,%rdx
 569:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
 56d:	50                   	push   %rax
 56e:	54                   	push   %rsp
 56f:	4c 8d 05 8a 02 00 00 	lea    0x28a(%rip),%r8        # 800 <__libc_csu_fini>
 576:	48 8d 0d 13 02 00 00 	lea    0x213(%rip),%rcx        # 790 <__libc_csu_init>
 57d:	48 8d 3d 45 01 00 00 	lea    0x145(%rip),%rdi        # 6c9 <main>
 584:	ff 15 56 0a 20 00    	callq  *0x200a56(%rip)        # 200fe0 <__libc_start_main@GLIBC_2.2.5>
 58a:	f4                   	hlt    
 58b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000000590 <deregister_tm_clones>:
 590:	48 8d 3d 91 0a 20 00 	lea    0x200a91(%rip),%rdi        # 201028 <__TMC_END__>
 597:	48 8d 05 91 0a 20 00 	lea    0x200a91(%rip),%rax        # 20102f <__TMC_END__+0x7>
 59e:	55                   	push   %rbp
 59f:	48 29 f8             	sub    %rdi,%rax
 5a2:	48 89 e5             	mov    %rsp,%rbp
 5a5:	48 83 f8 0e          	cmp    $0xe,%rax
 5a9:	76 15                	jbe    5c0 <deregister_tm_clones+0x30>
 5ab:	48 8b 05 36 0a 20 00 	mov    0x200a36(%rip),%rax        # 200fe8 <_ITM_deregisterTMCloneTable>
 5b2:	48 85 c0             	test   %rax,%rax
 5b5:	74 09                	je     5c0 <deregister_tm_clones+0x30>
 5b7:	5d                   	pop    %rbp
 5b8:	ff e0                	jmpq   *%rax
 5ba:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
 5c0:	5d                   	pop    %rbp
 5c1:	c3                   	retq   
 5c2:	0f 1f 40 00          	nopl   0x0(%rax)
 5c6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 5cd:	00 00 00 

00000000000005d0 <register_tm_clones>:
 5d0:	48 8d 3d 51 0a 20 00 	lea    0x200a51(%rip),%rdi        # 201028 <__TMC_END__>
 5d7:	48 8d 35 4a 0a 20 00 	lea    0x200a4a(%rip),%rsi        # 201028 <__TMC_END__>
 5de:	55                   	push   %rbp
 5df:	48 29 fe             	sub    %rdi,%rsi
 5e2:	48 89 e5             	mov    %rsp,%rbp
 5e5:	48 c1 fe 03          	sar    $0x3,%rsi
 5e9:	48 89 f0             	mov    %rsi,%rax
 5ec:	48 c1 e8 3f          	shr    $0x3f,%rax
 5f0:	48 01 c6             	add    %rax,%rsi
 5f3:	48 d1 fe             	sar    %rsi
 5f6:	74 18                	je     610 <register_tm_clones+0x40>
 5f8:	48 8b 05 f1 09 20 00 	mov    0x2009f1(%rip),%rax        # 200ff0 <_ITM_registerTMCloneTable>
 5ff:	48 85 c0             	test   %rax,%rax
 602:	74 0c                	je     610 <register_tm_clones+0x40>
 604:	5d                   	pop    %rbp
 605:	ff e0                	jmpq   *%rax
 607:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
 60e:	00 00 
 610:	5d                   	pop    %rbp
 611:	c3                   	retq   
 612:	0f 1f 40 00          	nopl   0x0(%rax)
 616:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 61d:	00 00 00 

0000000000000620 <__do_global_dtors_aux>:
 620:	80 3d 01 0a 20 00 00 	cmpb   $0x0,0x200a01(%rip)        # 201028 <__TMC_END__>
 627:	75 27                	jne    650 <__do_global_dtors_aux+0x30>
 629:	48 83 3d c7 09 20 00 	cmpq   $0x0,0x2009c7(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 630:	00 
 631:	55                   	push   %rbp
 632:	48 89 e5             	mov    %rsp,%rbp
 635:	74 0c                	je     643 <__do_global_dtors_aux+0x23>
 637:	48 8b 3d e2 09 20 00 	mov    0x2009e2(%rip),%rdi        # 201020 <__dso_handle>
 63e:	e8 0d ff ff ff       	callq  550 <.plt.got>
 643:	e8 48 ff ff ff       	callq  590 <deregister_tm_clones>
 648:	5d                   	pop    %rbp
 649:	c6 05 d8 09 20 00 01 	movb   $0x1,0x2009d8(%rip)        # 201028 <__TMC_END__>
 650:	f3 c3                	repz retq 
 652:	0f 1f 40 00          	nopl   0x0(%rax)
 656:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 65d:	00 00 00 

0000000000000660 <frame_dummy>:
 660:	48 8d 3d 81 07 20 00 	lea    0x200781(%rip),%rdi        # 200de8 <__JCR_END__>
 667:	48 83 3f 00          	cmpq   $0x0,(%rdi)
 66b:	75 0b                	jne    678 <frame_dummy+0x18>
 66d:	e9 5e ff ff ff       	jmpq   5d0 <register_tm_clones>
 672:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
 678:	48 8b 05 59 09 20 00 	mov    0x200959(%rip),%rax        # 200fd8 <_Jv_RegisterClasses>
 67f:	48 85 c0             	test   %rax,%rax
 682:	74 e9                	je     66d <frame_dummy+0xd>
 684:	55                   	push   %rbp
 685:	48 89 e5             	mov    %rsp,%rbp
 688:	ff d0                	callq  *%rax
 68a:	5d                   	pop    %rbp
 68b:	e9 40 ff ff ff       	jmpq   5d0 <register_tm_clones>

0000000000000690 <_Z14atomicFunctionv>:
 690:	55                   	push   %rbp
 691:	48 89 e5             	mov    %rsp,%rbp
 694:	48 83 ec 10          	sub    $0x10,%rsp
 698:	48 8d 45 fc          	lea    -0x4(%rbp),%rax
 69c:	48 89 c7             	mov    %rax,%rdi
 69f:	e8 50 00 00 00       	callq  6f4 <_ZNSt6atomicIiEC1Ev>
 6a4:	48 8d 45 fc          	lea    -0x4(%rbp),%rax
 6a8:	be 00 00 00 00       	mov    $0x0,%esi
 6ad:	48 89 c7             	mov    %rax,%rdi
 6b0:	e8 5b 00 00 00       	callq  710 <_ZNSt13__atomic_baseIiEaSEi>
 6b5:	48 8d 45 fc          	lea    -0x4(%rbp),%rax
 6b9:	be 00 00 00 00       	mov    $0x0,%esi
 6be:	48 89 c7             	mov    %rax,%rdi
 6c1:	e8 92 00 00 00       	callq  758 <_ZNSt13__atomic_baseIiEppEi>
 6c6:	90                   	nop
 6c7:	c9                   	leaveq 
 6c8:	c3                   	retq   

00000000000006c9 <main>:
 6c9:	55                   	push   %rbp
 6ca:	48 89 e5             	mov    %rsp,%rbp
 6cd:	b8 00 00 00 00       	mov    $0x0,%eax
 6d2:	5d                   	pop    %rbp
 6d3:	c3                   	retq   

00000000000006d4 <_ZStanSt12memory_orderSt23__memory_order_modifier>:
 6d4:	55                   	push   %rbp
 6d5:	48 89 e5             	mov    %rsp,%rbp
 6d8:	89 7d fc             	mov    %edi,-0x4(%rbp)
 6db:	89 75 f8             	mov    %esi,-0x8(%rbp)
 6de:	8b 55 fc             	mov    -0x4(%rbp),%edx
 6e1:	8b 45 f8             	mov    -0x8(%rbp),%eax
 6e4:	21 d0                	and    %edx,%eax
 6e6:	5d                   	pop    %rbp
 6e7:	c3                   	retq   

00000000000006e8 <_ZNSt13__atomic_baseIiEC1Ev>:
 6e8:	55                   	push   %rbp
 6e9:	48 89 e5             	mov    %rsp,%rbp
 6ec:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 6f0:	90                   	nop
 6f1:	5d                   	pop    %rbp
 6f2:	c3                   	retq   
 6f3:	90                   	nop

00000000000006f4 <_ZNSt6atomicIiEC1Ev>:
 6f4:	55                   	push   %rbp
 6f5:	48 89 e5             	mov    %rsp,%rbp
 6f8:	48 83 ec 10          	sub    $0x10,%rsp
 6fc:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 700:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 704:	48 89 c7             	mov    %rax,%rdi
 707:	e8 dc ff ff ff       	callq  6e8 <_ZNSt13__atomic_baseIiEC1Ev>
 70c:	90                   	nop
 70d:	c9                   	leaveq 
 70e:	c3                   	retq   
 70f:	90                   	nop

0000000000000710 <_ZNSt13__atomic_baseIiEaSEi>:
 710:	55                   	push   %rbp
 711:	48 89 e5             	mov    %rsp,%rbp
 714:	48 83 ec 30          	sub    $0x30,%rsp
 718:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
 71c:	89 75 d4             	mov    %esi,-0x2c(%rbp)
 71f:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
 723:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
 727:	8b 45 d4             	mov    -0x2c(%rbp),%eax
 72a:	89 45 f4             	mov    %eax,-0xc(%rbp)
 72d:	c7 45 f0 05 00 00 00 	movl   $0x5,-0x10(%rbp)
 734:	8b 45 f0             	mov    -0x10(%rbp),%eax
 737:	be ff ff 00 00       	mov    $0xffff,%esi
 73c:	89 c7                	mov    %eax,%edi
 73e:	e8 91 ff ff ff       	callq  6d4 <_ZStanSt12memory_orderSt23__memory_order_modifier>
 743:	89 45 ec             	mov    %eax,-0x14(%rbp)
 746:	8b 55 f4             	mov    -0xc(%rbp),%edx
 749:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 74d:	89 10                	mov    %edx,(%rax)
 74f:	0f ae f0             	mfence 
 752:	8b 45 d4             	mov    -0x2c(%rbp),%eax
 755:	c9                   	leaveq 
 756:	c3                   	retq   
 757:	90                   	nop

0000000000000758 <_ZNSt13__atomic_baseIiEppEi>:
 758:	55                   	push   %rbp
 759:	48 89 e5             	mov    %rsp,%rbp
 75c:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
 760:	89 75 e4             	mov    %esi,-0x1c(%rbp)
 763:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
 767:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
 76b:	c7 45 f4 01 00 00 00 	movl   $0x1,-0xc(%rbp)
 772:	c7 45 f0 05 00 00 00 	movl   $0x5,-0x10(%rbp)
 779:	8b 55 f4             	mov    -0xc(%rbp),%edx
 77c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 780:	f0 0f c1 10          	lock xadd %edx,(%rax)
 784:	89 d0                	mov    %edx,%eax
 786:	90                   	nop
 787:	5d                   	pop    %rbp
 788:	c3                   	retq   
 789:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000000790 <__libc_csu_init>:
 790:	41 57                	push   %r15
 792:	41 56                	push   %r14
 794:	41 89 ff             	mov    %edi,%r15d
 797:	41 55                	push   %r13
 799:	41 54                	push   %r12
 79b:	4c 8d 25 36 06 20 00 	lea    0x200636(%rip),%r12        # 200dd8 <__frame_dummy_init_array_entry>
 7a2:	55                   	push   %rbp
 7a3:	48 8d 2d 36 06 20 00 	lea    0x200636(%rip),%rbp        # 200de0 <__init_array_end>
 7aa:	53                   	push   %rbx
 7ab:	49 89 f6             	mov    %rsi,%r14
 7ae:	49 89 d5             	mov    %rdx,%r13
 7b1:	4c 29 e5             	sub    %r12,%rbp
 7b4:	48 83 ec 08          	sub    $0x8,%rsp
 7b8:	48 c1 fd 03          	sar    $0x3,%rbp
 7bc:	e8 5f fd ff ff       	callq  520 <_init>
 7c1:	48 85 ed             	test   %rbp,%rbp
 7c4:	74 20                	je     7e6 <__libc_csu_init+0x56>
 7c6:	31 db                	xor    %ebx,%ebx
 7c8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
 7cf:	00 
 7d0:	4c 89 ea             	mov    %r13,%rdx
 7d3:	4c 89 f6             	mov    %r14,%rsi
 7d6:	44 89 ff             	mov    %r15d,%edi
 7d9:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
 7dd:	48 83 c3 01          	add    $0x1,%rbx
 7e1:	48 39 dd             	cmp    %rbx,%rbp
 7e4:	75 ea                	jne    7d0 <__libc_csu_init+0x40>
 7e6:	48 83 c4 08          	add    $0x8,%rsp
 7ea:	5b                   	pop    %rbx
 7eb:	5d                   	pop    %rbp
 7ec:	41 5c                	pop    %r12
 7ee:	41 5d                	pop    %r13
 7f0:	41 5e                	pop    %r14
 7f2:	41 5f                	pop    %r15
 7f4:	c3                   	retq   
 7f5:	90                   	nop
 7f6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 7fd:	00 00 00 

0000000000000800 <__libc_csu_fini>:
 800:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000000804 <_fini>:
 804:	48 83 ec 08          	sub    $0x8,%rsp
 808:	48 83 c4 08          	add    $0x8,%rsp
 80c:	c3                   	retq   
