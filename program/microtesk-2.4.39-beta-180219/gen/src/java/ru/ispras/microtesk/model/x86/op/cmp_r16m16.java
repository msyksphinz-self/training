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

public final class cmp_r16m16 extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "cmp_r16m16",
          cmp_r16m16.class,
          null
          );
      addArgument("seg_ovrd", RSEG16.INFO);
      addArgument("op1", R16.INFO);
      addArgument("op2", MOFFS.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final RSEG16 seg_ovrd = (RSEG16) getArgument("seg_ovrd", args);
      final R16 op1 = (R16) getArgument("op1", args);
      final IsaPrimitive op2 = (IsaPrimitive) getArgument("op2", args);

      return new cmp_r16m16(seg_ovrd, op1, op2);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "cmp_r16m16",
          instruction.class,
          null
          );
      addArgument("seg_ovrd", RSEG16.INFO);
      addArgument("op1", R16.INFO);
      addArgument("op2", MOFFS.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final RSEG16 seg_ovrd = (RSEG16) getArgument("seg_ovrd", args);
      final R16 op1 = (R16) getArgument("op1", args);
      final IsaPrimitive op2 = (IsaPrimitive) getArgument("op2", args);

      return new instruction(
        new cmp_r16m16(
          seg_ovrd,
          op1,
          op2
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final RSEG16 seg_ovrd;
  public final R16 op1;
  public final IsaPrimitive op2;

  public cmp_r16m16(final RSEG16 seg_ovrd, final R16 op1, final IsaPrimitive op2) {
    assert RSEG16.INFO.isSupported(seg_ovrd);
    assert R16.INFO.isSupported(op1);
    assert MOFFS.INFO.isSupported(op2);

    this.seg_ovrd = seg_ovrd;
    this.op1 = op1;
    this.op2 = op2;

    addArgument("seg_ovrd", seg_ovrd);
    addArgument("op1", op1);
    addArgument("op2", op2);
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    vars__.mod.access().store(Data.valueOf(Type.CARD(2), 0x0));
    vars__.rm.access().store(Data.valueOf(Type.CARD(3), 0x0));
    op2.init(vars__);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("cmp %s, word%s", op1.text(vars__), op2.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("001%s11000111011%s%s%s%s", seg_ovrd.image(vars__), vars__.mod.access().load().toString(), op1.image(vars__), vars__.rm.access().load().toString(), op2.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    vars__.base.access().store(Data.zeroExtend(addR, seg_ovrd.access(pe__, vars__).load()).shiftLeft(Data.valueOf(addR, 0x4)));
    new sub_proc_word(new Immediate(op1.access(pe__, vars__).load().add(Data.valueOf(WORD, 0x0))), new Immediate(Location.concat(pe__.MEM.access(vars__.base.access().load().add(op2.access(pe__, vars__).load()).add(Data.valueOf(addR, 0x1))), pe__.MEM.access(vars__.base.access().load().add(op2.access(pe__, vars__).load()))))).execute(pe__, vars__);
    op2.execute(pe__, vars__);
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x3));
  }
}