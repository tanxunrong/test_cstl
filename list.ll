; ModuleID = 'list.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.list = type { i64, void (i8*)*, %struct.l_item*, %struct.l_item* }
%struct.l_item = type { i8*, i32, %struct.l_item*, %struct.l_item* }

; Function Attrs: nounwind uwtable
define %struct.list* @create_list() #0 {
  %dl = alloca %struct.list*, align 8
  %1 = call noalias i8* @malloc(i64 32) #2
  %2 = bitcast i8* %1 to %struct.list*
  store %struct.list* %2, %struct.list** %dl, align 8
  %3 = load %struct.list** %dl, align 8
  %4 = bitcast %struct.list* %3 to i8*
  call void @llvm.memset.p0i8.i64(i8* %4, i8 0, i64 32, i32 8, i1 false)
  %5 = load %struct.list** %dl, align 8
  %6 = getelementptr inbounds %struct.list* %5, i32 0, i32 1
  store void (i8*)* @free, void (i8*)** %6, align 8
  %7 = load %struct.list** %dl, align 8
  %8 = getelementptr inbounds %struct.list* %7, i32 0, i32 2
  store %struct.l_item* null, %struct.l_item** %8, align 8
  %9 = load %struct.list** %dl, align 8
  %10 = getelementptr inbounds %struct.list* %9, i32 0, i32 3
  store %struct.l_item* null, %struct.l_item** %10, align 8
  %11 = load %struct.list** %dl, align 8
  %12 = getelementptr inbounds %struct.list* %11, i32 0, i32 0
  store i64 0, i64* %12, align 8
  %13 = load %struct.list** %dl, align 8
  ret %struct.list* %13
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

; Function Attrs: nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture, i8, i64, i32, i1) #2

; Function Attrs: nounwind
declare void @free(i8*) #1

; Function Attrs: nounwind uwtable
define void @list_push(%struct.list* %dl, i8* %data, i64 %len) #0 {
  %1 = alloca %struct.list*, align 8
  %2 = alloca i8*, align 8
  %3 = alloca i64, align 8
  %node = alloca %struct.l_item*, align 8
  %cur = alloca %struct.l_item*, align 8
  store %struct.list* %dl, %struct.list** %1, align 8
  store i8* %data, i8** %2, align 8
  store i64 %len, i64* %3, align 8
  %4 = call noalias i8* @malloc(i64 32) #2
  %5 = bitcast i8* %4 to %struct.l_item*
  store %struct.l_item* %5, %struct.l_item** %node, align 8
  %6 = load %struct.l_item** %node, align 8
  %7 = bitcast %struct.l_item* %6 to i8*
  call void @llvm.memset.p0i8.i64(i8* %7, i8 0, i64 32, i32 8, i1 false)
  %8 = load i64* %3, align 8
  %9 = call noalias i8* @malloc(i64 %8) #2
  %10 = load %struct.l_item** %node, align 8
  %11 = getelementptr inbounds %struct.l_item* %10, i32 0, i32 0
  store i8* %9, i8** %11, align 8
  %12 = load %struct.l_item** %node, align 8
  %13 = getelementptr inbounds %struct.l_item* %12, i32 0, i32 0
  %14 = load i8** %13, align 8
  %15 = load i8** %2, align 8
  %16 = load i64* %3, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %14, i8* %15, i64 %16, i32 1, i1 false)
  %17 = load %struct.list** %1, align 8
  %18 = getelementptr inbounds %struct.list* %17, i32 0, i32 3
  %19 = load %struct.l_item** %18, align 8
  store %struct.l_item* %19, %struct.l_item** %cur, align 8
  %20 = load %struct.list** %1, align 8
  %21 = getelementptr inbounds %struct.list* %20, i32 0, i32 3
  %22 = load %struct.l_item** %21, align 8
  %23 = icmp eq %struct.l_item* %22, null
  br i1 %23, label %24, label %28

; <label>:24                                      ; preds = %0
  %25 = load %struct.l_item** %node, align 8
  %26 = load %struct.list** %1, align 8
  %27 = getelementptr inbounds %struct.list* %26, i32 0, i32 2
  store %struct.l_item* %25, %struct.l_item** %27, align 8
  br label %35

; <label>:28                                      ; preds = %0
  %29 = load %struct.l_item** %node, align 8
  %30 = load %struct.l_item** %cur, align 8
  %31 = getelementptr inbounds %struct.l_item* %30, i32 0, i32 3
  store %struct.l_item* %29, %struct.l_item** %31, align 8
  %32 = load %struct.l_item** %cur, align 8
  %33 = load %struct.l_item** %node, align 8
  %34 = getelementptr inbounds %struct.l_item* %33, i32 0, i32 2
  store %struct.l_item* %32, %struct.l_item** %34, align 8
  br label %35

; <label>:35                                      ; preds = %28, %24
  %36 = load %struct.l_item** %node, align 8
  %37 = load %struct.list** %1, align 8
  %38 = getelementptr inbounds %struct.list* %37, i32 0, i32 3
  store %struct.l_item* %36, %struct.l_item** %38, align 8
  %39 = load %struct.list** %1, align 8
  %40 = getelementptr inbounds %struct.list* %39, i32 0, i32 0
  %41 = load i64* %40, align 8
  %42 = add i64 %41, 1
  store i64 %42, i64* %40, align 8
  ret void
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #2

; Function Attrs: nounwind uwtable
define i8* @list_pop(%struct.list* %dl) #0 {
  %1 = alloca i8*, align 8
  %2 = alloca %struct.list*, align 8
  %cur = alloca %struct.l_item*, align 8
  %pop = alloca %struct.l_item*, align 8
  %data = alloca i8*, align 8
  store %struct.list* %dl, %struct.list** %2, align 8
  %3 = load %struct.list** %2, align 8
  %4 = getelementptr inbounds %struct.list* %3, i32 0, i32 0
  %5 = load i64* %4, align 8
  %6 = icmp eq i64 %5, 0
  br i1 %6, label %7, label %8

; <label>:7                                       ; preds = %0
  store i8* null, i8** %1
  br label %31

; <label>:8                                       ; preds = %0
  %9 = load %struct.list** %2, align 8
  %10 = getelementptr inbounds %struct.list* %9, i32 0, i32 3
  %11 = load %struct.l_item** %10, align 8
  store %struct.l_item* %11, %struct.l_item** %cur, align 8
  %12 = load %struct.l_item** %cur, align 8
  %13 = getelementptr inbounds %struct.l_item* %12, i32 0, i32 2
  %14 = load %struct.l_item** %13, align 8
  store %struct.l_item* %14, %struct.l_item** %cur, align 8
  %15 = load %struct.l_item** %cur, align 8
  %16 = getelementptr inbounds %struct.l_item* %15, i32 0, i32 3
  store %struct.l_item* null, %struct.l_item** %16, align 8
  %17 = load %struct.list** %2, align 8
  %18 = getelementptr inbounds %struct.list* %17, i32 0, i32 3
  %19 = load %struct.l_item** %18, align 8
  store %struct.l_item* %19, %struct.l_item** %pop, align 8
  %20 = load %struct.l_item** %pop, align 8
  %21 = getelementptr inbounds %struct.l_item* %20, i32 0, i32 2
  store %struct.l_item* null, %struct.l_item** %21, align 8
  %22 = load %struct.l_item** %cur, align 8
  %23 = load %struct.list** %2, align 8
  %24 = getelementptr inbounds %struct.list* %23, i32 0, i32 3
  store %struct.l_item* %22, %struct.l_item** %24, align 8
  %25 = load %struct.l_item** %pop, align 8
  %26 = getelementptr inbounds %struct.l_item* %25, i32 0, i32 0
  %27 = load i8** %26, align 8
  store i8* %27, i8** %data, align 8
  %28 = load %struct.l_item** %pop, align 8
  %29 = bitcast %struct.l_item* %28 to i8*
  call void @free(i8* %29) #2
  %30 = load i8** %data, align 8
  store i8* %30, i8** %1
  br label %31

; <label>:31                                      ; preds = %8, %7
  %32 = load i8** %1
  ret i8* %32
}

; Function Attrs: nounwind uwtable
define void @list_unshift(%struct.list* %dl, i8* %data, i64 %len) #0 {
  %1 = alloca %struct.list*, align 8
  %2 = alloca i8*, align 8
  %3 = alloca i64, align 8
  %node = alloca %struct.l_item*, align 8
  %cur = alloca %struct.l_item*, align 8
  store %struct.list* %dl, %struct.list** %1, align 8
  store i8* %data, i8** %2, align 8
  store i64 %len, i64* %3, align 8
  %4 = call noalias i8* @malloc(i64 32) #2
  %5 = bitcast i8* %4 to %struct.l_item*
  store %struct.l_item* %5, %struct.l_item** %node, align 8
  %6 = load %struct.l_item** %node, align 8
  %7 = bitcast %struct.l_item* %6 to i8*
  call void @llvm.memset.p0i8.i64(i8* %7, i8 0, i64 32, i32 8, i1 false)
  %8 = load i64* %3, align 8
  %9 = call noalias i8* @malloc(i64 %8) #2
  %10 = load %struct.l_item** %node, align 8
  %11 = getelementptr inbounds %struct.l_item* %10, i32 0, i32 0
  store i8* %9, i8** %11, align 8
  %12 = load %struct.l_item** %node, align 8
  %13 = getelementptr inbounds %struct.l_item* %12, i32 0, i32 0
  %14 = load i8** %13, align 8
  %15 = load i8** %2, align 8
  %16 = load i64* %3, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %14, i8* %15, i64 %16, i32 1, i1 false)
  %17 = load %struct.list** %1, align 8
  %18 = getelementptr inbounds %struct.list* %17, i32 0, i32 2
  %19 = load %struct.l_item** %18, align 8
  store %struct.l_item* %19, %struct.l_item** %cur, align 8
  %20 = load %struct.list** %1, align 8
  %21 = getelementptr inbounds %struct.list* %20, i32 0, i32 2
  %22 = load %struct.l_item** %21, align 8
  %23 = icmp eq %struct.l_item* %22, null
  br i1 %23, label %24, label %28

; <label>:24                                      ; preds = %0
  %25 = load %struct.l_item** %node, align 8
  %26 = load %struct.list** %1, align 8
  %27 = getelementptr inbounds %struct.list* %26, i32 0, i32 3
  store %struct.l_item* %25, %struct.l_item** %27, align 8
  br label %35

; <label>:28                                      ; preds = %0
  %29 = load %struct.l_item** %node, align 8
  %30 = load %struct.l_item** %cur, align 8
  %31 = getelementptr inbounds %struct.l_item* %30, i32 0, i32 3
  store %struct.l_item* %29, %struct.l_item** %31, align 8
  %32 = load %struct.l_item** %cur, align 8
  %33 = load %struct.l_item** %node, align 8
  %34 = getelementptr inbounds %struct.l_item* %33, i32 0, i32 2
  store %struct.l_item* %32, %struct.l_item** %34, align 8
  br label %35

; <label>:35                                      ; preds = %28, %24
  %36 = load %struct.l_item** %node, align 8
  %37 = load %struct.list** %1, align 8
  %38 = getelementptr inbounds %struct.list* %37, i32 0, i32 3
  store %struct.l_item* %36, %struct.l_item** %38, align 8
  %39 = load %struct.list** %1, align 8
  %40 = getelementptr inbounds %struct.list* %39, i32 0, i32 0
  %41 = load i64* %40, align 8
  %42 = add i64 %41, 1
  store i64 %42, i64* %40, align 8
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.4-1~exp1 (trunk) (based on LLVM 3.4)"}
