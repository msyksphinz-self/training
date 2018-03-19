/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.x86.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.x86.PE;
import ru.ispras.microtesk.model.x86.TempVars;
import ru.ispras.microtesk.model.x86.mode.*;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class pop_m16 extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "pop_m16",
          pop_m16.class,
          null
          );
      addArgument("seg_ovrd", RSEG16.INFO);
      addArgument("dst", MOFFS.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final RSEG16 seg_ovrd = (RSEG16) getArgument("seg_ovrd", args);
      final IsaPrimitive dst = (IsaPrimitive) getArgument("dst", args);

      return new pop_m16(seg_ovrd, dst);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "pop_m16",
          instruction.class,
          null
          );
      addArgument("seg_ovrd", RSEG16.INFO);
      addArgument("dst", MOFFS.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final RSEG16 seg_ovrd = (RSEG16) getArgument("seg_ovrd", args);
      final IsaPrimitive dst = (IsaPrimitive) getArgument("dst", args);

      return new instruction(
        new pop_m16(
          seg_ovrd,
          dst
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final RSEG16 seg_ovrd;
  public final IsaPrimitive dst;

  public pop_m16(final RSEG16 seg_ovrd, final IsaPrimitive dst) {
    assert RSEG16.INFO.isSupported(seg_ovrd);
    assert MOFFS.INFO.isSupported(dst);

    this.seg_ovrd = seg_ovrd;
    this.dst = dst;

    addArgument("seg_ovrd", seg_ovrd);
    addArgument("dst", dst);
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    vars__.mod.access().store(Data.valueOf(Type.CARD(2), 0x0));
    vars__.rm.access().store(Data.valueOf(Type.CARD(3), 0x0));
    dst.init(vars__);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("pop %s:%s", seg_ovrd.text(vars__), dst.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("001%s11010001111%s000%s%s", seg_ovrd.image(vars__), vars__.mod.access().load().toString(), vars__.rm.access().load().toString(), dst.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    vars__.base.access().store(Data.zeroExtend(addR, seg_ovrd.access(pe__, vars__).load()).shiftLeft(Data.valueOf(addR, 0x4)));
    pe__.MEM.access(vars__.base.access().load().add(dst.access(pe__, vars__).load())).store(pe__.MEM.access(Data.zeroExtend(addR, pe__.SS.access().load()).shiftLeft(Data.valueOf(addR, 0x4)).add(Data.signExtend(addR, Data.cast(SWORD, pe__.SP.access().load())))).load());
    pe__.MEM.access(vars__.base.access().load().add(dst.access(pe__, vars__).load()).add(Data.valueOf(addR, 0x1))).store(pe__.MEM.access(Data.zeroExtend(addR, pe__.SS.access().load()).shiftLeft(Data.valueOf(addR, 0x4)).add(Data.signExtend(addR, Data.cast(SWORD, pe__.SP.access().load().add(Data.valueOf(WORD, 0x1)))))).load());
    pe__.SP.access().store(pe__.SP.access().load().add(Data.valueOf(WORD, 0x2)));
    dst.execute(pe__, vars__);
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x3));
  }
}